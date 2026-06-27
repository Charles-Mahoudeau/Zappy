/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World
*/

#include "World.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <format>
#include <memory>
#include <optional>
#include <random>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "EntityDatabase.hpp"
#include "Event.hpp"
#include "ResourceType.hpp"
#include "Tile.hpp"
#include "entity/Egg.hpp"
#include "entity/Player.hpp"
#include "zappy/server/game/Grid.hpp"
#include "zappy/shared/exception/Exception.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"
#include "zappy/shared/exception/OutOfRange.hpp"
#include "zappy/shared/io/Logger.hpp"
#include "zappy/shared/math/Direction.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
World::World(const math::Vector2u size, std::optional<io::Logger> logger) : _grid{size}, _logger{std::move(logger)} {
    generateResourceThresholds();
    if (_logger.has_value()) {
        _logger->info("World initialized.");
    }
}

void World::update() {
    // TODO: Use timer class when ready.
    --_nextMajorTick;
    if (_nextMajorTick != 0) {
        return;
    }
    spawnResources();
    _nextMajorTick = kMajorTickInterval;
}

math::Vector2u World::size() const { return _grid.size(); }

const EntityDatabase& World::entityDatabase() const { return _entityDatabase; }

const Grid& World::grid() const { return _grid; }

std::uint64_t World::countResources(const ResourceType type) const {
    std::uint64_t count = 0;

    for (const Tile& tile : _grid.tiles()) {
        count += tile.inventory().resourceCount(type);
    }
    return count;
}

std::uint64_t World::spawnEgg(std::uint64_t playerId, const std::string_view teamName) {
    const std::uint64_t eggId =
        _entityDatabase.insert(std::make_unique<entity::Egg>(_grid, *this, std::string{teamName}, playerId));

    placeEggRandom(eggId);
    return eggId;
}

std::uint64_t World::spawnEgg(const std::string_view teamName) {
    const std::uint64_t eggId =
        _entityDatabase.insert(std::make_unique<entity::Egg>(_grid, *this, std::string{teamName}));

    placeEggRandom(eggId);
    return eggId;
}

void World::spawnResource(const ResourceType type) {
    // FIXME: "resources should be evenly spread across the map." instead of random
    Tile& tile = randomTile();

    tile.inventory().addResource(type);
    pushEvent(TileInventoryEvent{
        .position = tile.position(),
        .inventory = tile.inventory(),
    });
}

void World::spawnStartEggs(const std::span<const std::string_view> teams, const std::uint8_t playersPerTeam) {
    for (const std::string_view teamName : teams) {
        for (std::uint16_t i = 0; i < playersPerTeam; ++i) {
            std::ignore = spawnEgg(teamName);
        }
    }
    if (_logger.has_value()) {
        _logger->info("Start eggs spawned.");
    }
}

void World::spawnStartEggs(std::span<const std::string> teams, const std::uint8_t playersPerTeam) {
    std::vector<std::string_view> views{teams.begin(), teams.end()};

    spawnStartEggs(views, playersPerTeam);
}

std::expected<std::uint64_t, std::string> World::hatchRandomEgg(const std::string_view teamName) {
    std::vector<const entity::Egg*> eggs;

    for (const entity::Egg* egg : _entityDatabase.viewAll<entity::Egg>()) {
        if (egg != nullptr && egg->teamName() == teamName) {
            eggs.push_back(egg);
        }
    }
    if (eggs.empty()) {
        return std::unexpected{"No eggs available to hatch."};
    }

    std::uniform_int_distribution<std::size_t> distribution{0, eggs.size() - 1};
    const entity::Egg* egg = eggs.at(distribution(_randomEngine));

    if (egg == nullptr) {
        return std::unexpected{"Egg is null (this should never happen)."};
    }

    const std::optional<std::uint64_t> eggIdOpt = _entityDatabase.id(*egg);

    if (!eggIdOpt.has_value()) {
        return std::unexpected{"Egg was retrieved from the database, but its id is null (this should never happen)."};
    }

    Tile* parentTile = _grid.tile(*eggIdOpt);

    if (parentTile == nullptr) {
        return std::unexpected{"Egg is not on a tile."};
    }

    _entityDatabase.remove(*eggIdOpt);
    parentTile->removeEntity(*eggIdOpt);

    const std::uint64_t playerId =
        _entityDatabase.insert(std::make_unique<entity::Player>(_grid, *this, std::string{teamName}));

    parentTile->addEntity(playerId);
    pushEvent(PlayerConnectionEvent{
        .playerId = playerId,
        .position = parentTile->position(),
        .orientation = math::Direction::kNorth,
        .level = 0,
        .teamName = std::string{teamName},
    });
    pushEvent(EggConnectionEvent{
        .eggId = *eggIdOpt,
    });
    return playerId;
}

std::uint64_t World::eggCount(std::string_view teamName) const {
    return std::ranges::count_if(this->entityDatabase().viewAll<entity::Egg>(),
                                 [teamName](const entity::Egg* egg) { return egg->teamName() == teamName; });
}

EntityDatabase::EntityView<const entity::Player> World::players(const std::string_view teamName) const {
    return _entityDatabase.viewAll<entity::Player>() |
           std::views::filter([teamName](const entity::Player* player) { return player->teamName() == teamName; });
}

EntityDatabase::EntityView<entity::Player> World::players(const std::string_view teamName) {
    return _entityDatabase.viewAll<entity::Player>() |
           std::views::filter([teamName](const entity::Player* player) { return player->teamName() == teamName; });
}

const entity::Player* World::player(const std::uint64_t id) const { return _entityDatabase.query<entity::Player>(id); }

entity::Player* World::player(const std::uint64_t id) { return _entityDatabase.query<entity::Player>(id); }

void World::remove(const std::uint64_t entityId) {
    _grid.remove(entityId);
    _entityDatabase.remove(entityId);
}

bool World::hasEvents() const { return !_events.empty(); }

Event World::popEvent() {
    Event event{std::move(_events.back())};

    _events.pop_front();
    return event;
}

void World::pushEvent(Event event) { _events.push_back(std::move(event)); }

std::optional<World::IncantationSnapshot> World::beginIncantation(const std::uint64_t playerId) {
    const entity::Player* incantationPlayer = player(playerId);

    if (incantationPlayer == nullptr) {
        return std::nullopt;
    }
    if (incantationPlayer->level() >= entity::Player::kMaxLevel) {
        return std::nullopt;
    }

    const Tile& tile = _grid.tile(incantationPlayer->position());
    std::vector<std::uint64_t> players;

    for (const std::uint64_t entityId : tile.entities()) {
        if (_entityDatabase.is<entity::Player>(entityId)) {
            players.push_back(entityId);
        }
    }

    IncantationSnapshot snapshot{
        .position = tile.position(),
        .level = incantationPlayer->level(),
        .playerId = incantationPlayer->id(),
        .playerIds = players,
    };

    if (!verifyIncantationRequirements(snapshot).has_value()) {
        return std::nullopt;
    }
    pushEvent(IncantationBeginEvent{
        .position = snapshot.position,
        .level = snapshot.level,
        .playerIds = snapshot.playerIds,
    });
    return snapshot;
}

bool World::endIncantation(const IncantationSnapshot& snapshot) {
    if (!verifyIncantationRequirements(snapshot).has_value()) {
        pushEvent(IncantationEndEvent{
            .position = snapshot.position,
            .success = false,
        });
        return false;
    }

    const auto& [players, resources] = incantationRequirements(snapshot.level);

    Tile& tile = _grid.tile(snapshot.position);

    tile.inventory() -= resources;
    for (const std::uint64_t playerId : snapshot.playerIds) {
        entity::Player* incantationPlayer = player(playerId);

        if (incantationPlayer == nullptr) {
            continue;
        }
        std::ignore = incantationPlayer->levelUp();
    }
    pushEvent(IncantationEndEvent{
        .position = snapshot.position,
        .success = true,
    });
    return true;
}

const std::unordered_map<ResourceType, float>& World::resourceDensities() {
    using enum ResourceType;

    static const std::unordered_map<ResourceType, float> resourceDensities = {
        {kFood, 0.5F},     {kLinemate, 0.3F}, {kDeraumere, 0.15F}, {kSibur, 0.1F},
        {kMendiane, 0.1F}, {kPhiras, 0.08F},  {kThystame, 0.05F},
    };
    return resourceDensities;
}

void World::spawnResources() {
    for (const auto& [resourceType, quantity] : _resourceThresholds) {
        for (std::uint64_t count = countResources(resourceType); count < quantity; ++count) {
            spawnResource(resourceType);
        }
    }
    if (_logger.has_value()) {
        _logger->info("Resources spawned.");
    }
}

const World::IncantationRequirements& World::incantationRequirements(const std::uint8_t level) {
    static std::unordered_map<std::uint8_t, IncantationRequirements> requirements = {
        {
            1,
            IncantationRequirements{
                .players = 1,
                .resources = Inventory{{
                    .linemate = 1,
                }},
            },
        },
        {
            2,
            IncantationRequirements{
                .players = 2,
                .resources = Inventory{{
                    .linemate = 1,
                    .deraumere = 1,
                    .sibur = 1,
                }},
            },
        },
        {
            3,
            IncantationRequirements{
                .players = 2,
                .resources = Inventory{{
                    .linemate = 2,
                    .sibur = 1,
                    .phiras = 2,
                }},
            },
        },
        {
            4,
            IncantationRequirements{
                .players = 4,
                .resources = Inventory{{
                    .linemate = 1,
                    .deraumere = 1,
                    .sibur = 2,
                    .phiras = 1,
                }},
            },
        },
        {
            5,
            IncantationRequirements{
                .players = 4,
                .resources = Inventory{{
                    .linemate = 1,
                    .deraumere = 2,
                    .sibur = 1,
                    .mendiane = 3,
                }},
            },
        },
        {
            6,
            IncantationRequirements{
                .players = 6,
                .resources = Inventory{{
                    .linemate = 1,
                    .deraumere = 2,
                    .sibur = 3,
                    .phiras = 1,
                }},
            },
        },
        {
            7,
            IncantationRequirements{
                .players = 6,
                .resources = Inventory{{
                    .linemate = 2,
                    .deraumere = 2,
                    .sibur = 2,
                    .mendiane = 2,
                    .phiras = 2,
                    .thystame = 1,
                }},
            },
        },
    };
    const auto it = requirements.find(level);

    if (it == requirements.end()) {
        throw exception::OutOfRange{"invalid incantation level"};
    }
    return it->second;
}

Tile& World::randomTile() {
    const std::span<Tile> tiles = _grid.tiles();
    std::uniform_int_distribution<std::size_t> distribution{0, tiles.size() - 1};

    return tiles[distribution(_randomEngine)];  // NOLINT(*-pro-bounds-avoid-unchecked-container-access)
}

void World::generateResourceThresholds() {
    _resourceThresholds.clear();
    for (const auto& [resourceType, density] : resourceDensities()) {
        _resourceThresholds[resourceType] =
            static_cast<std::uint64_t>(std::ceil(static_cast<float>(_grid.size().x * _grid.size().y) * density));
    }
    if (_logger.has_value()) {
        _logger->info("Resources thresholds generated.");
    }
}

bool World::playerTake(entity::Player* player, ResourceType resource) {
    const math::Vector2u pos = player->position();
    Tile& tile = this->_grid.tile(pos);

    if (tile.inventory().resourceCount(resource) == 0) {
        return false;
    }
    tile.inventory().removeResource(resource);
    player->take(resource);
    this->pushEvent(PlayerResourceCollectEvent{.playerId = player->id(), .resourceType = resource});
    this->pushEvent(TileInventoryEvent{.position = tile.position(), .inventory = tile.inventory()});

    return true;
}

bool World::playerDrop(entity::Player* player, ResourceType resource) {
    try {
        if (!player->drop(resource)) {
            return false;
        }
        Tile& tile = this->_grid.tile(player->position());
        tile.inventory().addResource(resource);
        this->pushEvent(PlayerResourceDropEvent{.playerId = player->id(), .resourceType = resource});
        this->pushEvent(TileInventoryEvent{.position = tile.position(), .inventory = tile.inventory()});
    } catch (const exception::Exception& err) {
        if (this->_logger.has_value()) {
            this->_logger->error(err.what());
        }
        return false;
    }

    return true;
}

void World::placeEggRandom(const std::uint64_t eggId) {
    const entity::Egg* egg = _entityDatabase.query<entity::Egg>(eggId);

    if (egg == nullptr) {
        throw exception::InvalidArgument{"trying to place an egg that does not exist"};
    }

    Tile& tile = randomTile();

    tile.addEntity(eggId);
    pushEvent(EggLaidEvent{
        .playerId = egg->parentPlayerId(),
        .eggId = eggId,
        .position = tile.position(),
    });
    if (_logger.has_value()) {
        std::string playerInfo;

        if (egg->parentPlayerId().has_value()) {
            playerInfo = std::format(" by player #{}", egg->parentPlayerId().value());
        }
        _logger->info(std::format("Spawned egg #{} for team {}{} at ({}, {})", eggId, egg->teamName(), playerInfo,
                                  tile.position().x, tile.position().y));
    }
}

std::expected<void, std::string> World::verifyIncantationRequirements(const IncantationSnapshot& snapshot) const {
    const entity::Player* incantationPlayer = player(snapshot.playerId);

    if (incantationPlayer == nullptr) {
        return std::unexpected{"player does not exist"};
    }
    if (incantationPlayer->level() != snapshot.level) {
        return std::unexpected{"player is not at the right level"};
    }

    const auto& [playersNecessary, resourcesNecessary] = incantationRequirements(snapshot.level);

    if (snapshot.playerIds.size() < playersNecessary) {
        return std::unexpected{"not enough players"};
    }
    for (const std::uint64_t playerId : snapshot.playerIds) {
        const entity::Player* participant = player(playerId);

        if (participant == nullptr) {
            return std::unexpected{"participant does not exist"};
        }
        if (participant->level() != snapshot.level) {
            return std::unexpected{"participant is not at the right level"};
        }
        if (participant->position() != snapshot.position) {
            return std::unexpected{"participant is not at the right position"};
        }
    }
    if (const Tile& incantationTile = _grid.tile(snapshot.position);
        !incantationTile.inventory().canAfford(resourcesNecessary)) {
        return std::unexpected{"not enough resources"};
    }
    return {};
}
}  // namespace zappy::server::game
