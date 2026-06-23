/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World
*/

#include "World.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <format>
#include <limits>
#include <memory>
#include <optional>
#include <random>
#include <ranges>
#include <string>
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
#include "zappy/shared/exception/InvalidArgument.hpp"
#include "zappy/shared/exception/InvalidState.hpp"
#include "zappy/shared/exception/OutOfRange.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
World::World(Config config) : _config{std::move(config)} {
    const std::uint32_t size = _config.size.x * _config.size.y;

    if (size == 0) {
        throw exception::OutOfRange{"trying to create world with 0 width or height"};
    }
    _tiles.reserve(size);
    for (std::uint32_t i = 0; i < size; ++i) {
        _tiles.emplace_back(math::Vector2u{i % _config.size.x, i / _config.size.x});
    }
    generateResourceThresholds();
    spawnStartEggs();
    if (_config.logger) {
        _config.logger->info("World initialized.");
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

math::Vector2u World::size() const { return _config.size; }

const EntityDatabase& World::entityDatabase() const { return _entityDatabase; }

EntityDatabase& World::entityDatabase() { return _entityDatabase; }

const Tile& World::tile(const math::Vector2u pos) const {
    if (!isInBounds(pos)) {
        throw exception::OutOfRange{"trying to access tile out of bounds"};
    }
    return _tiles.at((pos.y * _config.size.x) + pos.x);
}

Tile& World::tile(const math::Vector2u position) {
    if (!isInBounds(position)) {
        throw exception::OutOfRange{"trying to access tile out of bounds"};
    }
    return _tiles.at((position.y * _config.size.x) + position.x);
}

const Tile* World::tile(const std::uint64_t entityId) const {
    for (const auto& tile : _tiles) {
        if (tile.hasEntity(entityId)) {
            return &tile;
        }
    }
    return nullptr;
}

Tile* World::tile(const std::uint64_t entityId) {
    for (auto& tile : _tiles) {
        if (tile.hasEntity(entityId)) {
            return &tile;
        }
    }
    return nullptr;
}

std::uint64_t World::countResources(const ResourceType type) const {
    std::uint64_t count = 0;

    for (const Tile& tile : _tiles) {
        count += tile.inventory().resourceCount(type);
    }
    return count;
}

std::uint64_t World::spawnEgg(std::uint64_t playerId, const std::string_view teamName) {
    const std::uint64_t eggId =
        _entityDatabase.insert(std::make_unique<entity::Egg>(*this, *this, std::string{teamName}));
    Tile& tile = randomTile();

    tile.addEntity(eggId);
    pushEvent(EggLaidEvent{
        .playerId =
            playerId == std::numeric_limits<decltype(playerId)>::max() ? std::nullopt : std::make_optional(playerId),
        .eggId = eggId,
        .position = tile.position(),
    });
    if (_config.logger) {
        _config.logger->info(std::format("Spawned egg #{} for team #{} at ({}, {})", eggId, teamName, tile.position().x,
                                         tile.position().y));
    }
    return eggId;
}

std::uint64_t World::spawnEgg(const std::string_view teamName) {
    // That's a shitty solution but it works...
    return spawnEgg(std::numeric_limits<std::uint64_t>::max(), teamName);
}

void World::spawnResource(const ResourceType type) {
    Tile& tile = randomTile();

    tile.inventory().addResource(type);
    pushEvent(TileInventoryEvent{
        .position = {},
        .inventory = tile.inventory(),
    });
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

    Tile* parentTile = tile(*eggIdOpt);

    if (parentTile == nullptr) {
        return std::unexpected{"Egg is not on a tile."};
    }

    _entityDatabase.remove(*eggIdOpt);
    parentTile->removeEntity(*eggIdOpt);

    const std::uint64_t playerId =
        _entityDatabase.insert(std::make_unique<entity::Player>(*this, *this, std::string{teamName}));

    parentTile->addEntity(playerId);
    // TODO: Send new player event
    pushEvent(EggConnectionEvent{
        .eggId = *eggIdOpt,
    });
    return playerId;
}

EntityDatabase::EntityView<const entity::Player> World::players(const std::string_view teamName) const {
    return _entityDatabase.viewAll<entity::Player>() |
           std::views::filter([teamName](const entity::Player* player) { return player->teamName() == teamName; });
}

EntityDatabase::EntityView<entity::Player> World::players(const std::string_view teamName) {
    return _entityDatabase.viewAll<entity::Player>() |
           std::views::filter([teamName](const entity::Player* player) { return player->teamName() == teamName; });
}

std::optional<math::Vector2u> World::position(const std::uint64_t entityId) const {
    const Tile* entityTile = tile(entityId);

    if (entityTile == nullptr) {
        return std::nullopt;
    }
    return entityTile->position();
}

void World::setPosition(const std::uint16_t entityId, const math::Vector2u position) {
    if (!isInBounds(position)) {
        throw exception::InvalidArgument{"position is out of bounds"};
    }

    Tile* sourceTile = tile(entityId);

    if (sourceTile == nullptr) {
        throw exception::InvalidState{"entity is not on a tile"};
    }
    if (!sourceTile->removeEntity(entityId)) {
        throw exception::InvalidState{"entity is not on the source tile (this should never happen)"};
    }

    Tile& destinationTile = tile(position);

    destinationTile.addEntity(entityId);
}

void World::remove(const std::uint64_t entityId) {
    // TODO: Do something, I guess?
}

bool World::hasEvents() const { return !_events.empty(); }

Event World::popEvent() {
    Event event{std::move(_events.back())};

    _events.pop_back();
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

bool World::isInBounds(const math::Vector2u position) const {
    return position.x < _config.size.x && position.y < _config.size.y;
}

void World::spawnStartEggs() {
    for (std::uint16_t teamId = 0; teamId < _config.teamCount; ++teamId) {
        for (std::uint16_t i = 0; i < _config.playersPerTeam; ++i) {
            // TODO: Update spawn mechanic
            std::ignore = spawnEgg("<TODO>");
        }
    }
    if (_config.logger) {
        _config.logger->info("Start eggs spawned.");
    }
}

void World::spawnResources() {
    for (const auto& [resourceType, quantity] : _resourceThresholds) {
        for (std::uint64_t count = countResources(resourceType); count < quantity; ++count) {
            spawnResource(resourceType);
        }
    }
    if (_config.logger) {
        _config.logger->info("Resources spawned.");
    }
}

Tile& World::randomTile() {
    std::uniform_int_distribution<std::size_t> distribution{0, _tiles.size() - 1};

    return _tiles.at(distribution(_randomEngine));
}

void World::generateResourceThresholds() {
    _resourceThresholds.clear();
    for (const auto& [resourceType, density] : resourceDensities()) {
        _resourceThresholds[resourceType] =
            static_cast<std::uint64_t>(std::ceil(static_cast<float>(_config.size.x * _config.size.y) * density));
    }
    if (_config.logger) {
        _config.logger->info("Resources thresholds generated.");
    }
}
}  // namespace zappy::server::game
