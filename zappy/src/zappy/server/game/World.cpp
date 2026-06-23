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
#include "ResourceType.hpp"
#include "Tile.hpp"
#include "entity/Egg.hpp"
#include "entity/Player.hpp"
#include "entity/Resource.hpp"
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
    auto resources = _entityDatabase.viewAll<entity::Resource>();
    std::uint64_t count = 0;

    for (const auto& resource : resources) {
        if (resource->type() == type) {
            ++count;
        }
    }
    return count;
}

std::uint64_t World::spawnEgg(std::uint16_t teamId) {
    const std::uint64_t eggId = _entityDatabase.insert(std::make_unique<entity::Egg>(teamId));
    Tile& tile = randomTile();

    tile.addEntity(eggId);
    if (_config.logger) {
        _config.logger->info(std::format("Spawned egg #{} for team #{} at ({}, {})", eggId, teamId, tile.position().x,
                                         tile.position().y));
    }
    return eggId;
}

std::uint64_t World::spawnResource(ResourceType type) {
    const std::uint64_t entityId = _entityDatabase.insert(std::make_unique<entity::Resource>(type));

    randomTile().addEntity(entityId);
    return entityId;
}

std::expected<std::uint64_t, std::string> World::hatchRandomEgg(const std::uint16_t teamId) {
    std::vector<const entity::Egg*> eggs;

    for (const entity::Egg* egg : _entityDatabase.viewAll<entity::Egg>()) {
        if (egg != nullptr && egg->teamId() == teamId) {
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

    const std::uint64_t playerId = _entityDatabase.insert(std::make_unique<entity::Player>(teamId));

    parentTile->addEntity(playerId);
    return playerId;
}

EntityDatabase::EntityView<const entity::Player> World::players(const std::uint16_t teamId) const {
    return _entityDatabase.viewAll<entity::Player>() |
           std::views::filter([teamId](const entity::Player* player) { return player->teamId() == teamId; });
}

EntityDatabase::EntityView<entity::Player> World::players(const std::uint16_t teamId) {
    return _entityDatabase.viewAll<entity::Player>() |
           std::views::filter([teamId](const entity::Player* player) { return player->teamId() == teamId; });
}

std::optional<math::Vector2u> World::position(const std::uint64_t entityId) const {
    const Tile* entityTile = tile(entityId);

    if (entityTile == nullptr) {
        return std::nullopt;
    }
    return entityTile->position();
}

void World::moveTo(const std::uint64_t entityId, const math::Vector2u position) {
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

math::Vector2u World::moveBy(const std::uint64_t entityId, const math::Vector2i delta) {
    Tile* sourceTile = tile(entityId);

    if (sourceTile == nullptr) {
        throw exception::InvalidState{"entity is not on a tile"};
    }
    if (!sourceTile->removeEntity(entityId)) {
        throw exception::InvalidState{"entity is not on the source tile (this should never happen)"};
    }

    const math::Vector2i newPositionRaw = static_cast<math::Vector2i>(sourceTile->position()) + delta;
    const math::Vector2i newPositionWrapped = newPositionRaw.wrapped(static_cast<math::Vector2i>(_config.size));
    const auto newPosition = static_cast<math::Vector2u>(newPositionWrapped);
    Tile& destinationTile = tile(newPosition);

    destinationTile.addEntity(entityId);
    return newPosition;
}

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
            std::ignore = spawnEgg(teamId);
        }
    }
    if (_config.logger) {
        _config.logger->info("Start eggs spawned.");
    }
}

void World::spawnResources() {
    for (const auto& [resourceType, quantity] : _resourceThresholds) {
        for (std::uint64_t count = countResources(resourceType); count < quantity; ++count) {
            std::ignore = spawnResource(resourceType);
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
