/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World
*/

#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "EntityDatabase.hpp"
#include "IEntity.hpp"
#include "ResourceType.hpp"
#include "Tile.hpp"
#include "zappy/shared/io/Logger.hpp"

namespace zappy::server::game {
class World {
  public:
    struct Config {
        std::uint16_t width;
        std::uint16_t height;
        std::uint16_t teamCount;
        std::uint16_t playersPerTeam;
        std::optional<io::Logger> logger;
    };

    static constexpr std::uint16_t kMajorTickInterval{20};

    explicit World(Config config);
    ~World() = default;

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    World(World&&) = delete;
    World& operator=(World&&) = delete;

    /// @brief Updates the world state.
    void update();

    /// @brief Returns the width of the world.
    /// @return The width of the world.
    [[nodiscard]] std::uint16_t width() const;

    /// @brief Returns the height of the world.
    /// @return The height of the world.
    [[nodiscard]] std::uint16_t height() const;

    /// @brief Returns a reference to the entity database.
    /// @return A reference to the entity database.
    [[nodiscard]] const EntityDatabase& entityDatabase() const;

    /// @brief Returns a reference to the entity database.
    /// @return A reference to the entity database.
    [[nodiscard]] EntityDatabase& entityDatabase();

    /// @brief Returns a reference to the tile at the specified coordinates.
    /// @param x The x-coordinate of the tile.
    /// @param y The y-coordinate of the tile.
    /// @return A reference to the tile at the specified coordinates.
    [[nodiscard]] const Tile& tile(std::uint16_t x, std::uint16_t y) const;

    /// @brief Returns a reference to the tile at the specified coordinates.
    /// @param x The x-coordinate of the tile.
    /// @param y The y-coordinate of the tile.
    /// @return A reference to the tile at the specified coordinates.
    [[nodiscard]] Tile& tile(std::uint16_t x, std::uint16_t y);

    /// @brief Returns a reference to the tile at the specified entity id.
    /// @param entityId The id of the entity.
    /// @return A reference to the tile at the specified coordinates.
    [[nodiscard]] const Tile* tile(std::uint64_t entityId) const;

    /// @brief Returns a reference to the tile at the specified entity id.
    /// @param entityId The id of the entity.
    /// @return A reference to the tile at the specified coordinates.
    [[nodiscard]] Tile* tile(std::uint64_t entityId);

    /// @brief Returns the number of entities of type T in the world.
    /// @tparam T The type of entity to count.
    /// @return The number of entities of type T in the world.
    template <IsEntity T>
    [[nodiscard]] std::uint64_t count() const;

    /// @brief Returns the number of resources of the specified type in the world.
    /// @param type The type of resource to count.
    /// @return The number of resources of the specified type in the world.
    [[nodiscard]] std::uint64_t countResources(ResourceType type) const;

    /// @brief Spawns an egg for the specified team.
    /// @param teamId The ID of the team to spawn the egg for.
    /// @return The ID of the spawned egg.
    [[nodiscard]] std::uint64_t spawnEgg(std::uint16_t teamId);

    /// @brief Spawns a resource of the specified type.
    /// @param type The type of resource to spawn.
    /// @return The ID of the spawned resource.
    [[nodiscard]] std::uint64_t spawnResource(ResourceType type);

    /// @brief Hatches a random egg for the specified team.
    /// @param teamId The ID of the team to hatch the egg for.
    /// @return The ID of the new player, or an error message if no egg could be hatched.
    std::expected<std::uint16_t, std::string> hatchRandomEgg(std::uint16_t teamId);

  private:
    /// @brief Returns the resource densities for the world.
    /// @return A map of resource types to their densities.
    [[nodiscard]] static const std::unordered_map<ResourceType, float>& resourceDensities();

    /// @brief Returns true if the specified coordinates are within the bounds of the world.
    /// @param x The x-coordinate to check.
    /// @param y The y-coordinate to check.
    /// @return True if the specified coordinates are within the bounds of the world, false otherwise.
    [[nodiscard]] bool isInBounds(std::uint16_t x, std::uint16_t y) const;

    /// @brief Spawns the initial eggs in the world.
    void spawnStartEggs();

    /// @brief Spawns resources in the world to meet the threshold.
    void spawnResources();

    /// @brief Returns a random tile in the world.
    /// @return A reference to a random tile in the world.
    [[nodiscard]] Tile& randomTile();

    /// @brief Generates the resource thresholds for the world.
    void generateResourceThresholds();

    std::random_device _randomDevice;
    std::mt19937 _randomEngine{_randomDevice()};
    Config _config;
    EntityDatabase _entityDatabase;
    std::vector<Tile> _tiles;
    std::unordered_map<ResourceType, std::uint16_t> _resourceThresholds;
    std::uint16_t _nextMajorTick{kMajorTickInterval};
};

template <IsEntity T>
std::uint64_t World::count() const {
    return _entityDatabase.countAll<T>();
}
}  // namespace zappy::server::game
