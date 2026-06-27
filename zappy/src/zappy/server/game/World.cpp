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
#include "zappy/shared/io/Logger.hpp"
#include "zappy/shared/math/Direction.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
World::World(const math::Vector2u size, Timer& timer, std::optional<io::Logger> logger)
    : _grid{size}, _timer{timer}, _logger{std::move(logger)} {
    generateResourceThresholds();
    if (_logger.has_value()) {
        _logger->info("World initialized.");
    }
    _resourceSpawnTimerId = _timer.get().scheduleEvery(
        kFoodRegenerationInterval, [this] { spawnResources(); }, [this] { return _resourcesDirty; });
}

World::~World() { _timer.get().unschedule(_resourceSpawnTimerId); }

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
        _entityDatabase.insert(std::make_unique<entity::Egg>(_timer, _grid, *this, std::string{teamName}, playerId));

    placeEggRandom(eggId);
    return eggId;
}

std::uint64_t World::spawnEgg(const std::string_view teamName) {
    const std::uint64_t eggId =
        _entityDatabase.insert(std::make_unique<entity::Egg>(_timer, _grid, *this, std::string{teamName}));

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

void World::markResourcesDirty() { _resourcesDirty = true; }

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
        _entityDatabase.insert(std::make_unique<entity::Player>(_timer, _grid, *this, std::string{teamName}));

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
    Event event{std::move(_events.front())};

    _events.pop_front();
    return event;
}

void World::pushEvent(Event event) { _events.push_back(std::move(event)); }

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
    _resourcesDirty = false;
    if (_logger.has_value()) {
        _logger->info("Resources spawned.");
    }
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
}  // namespace zappy::server::game
