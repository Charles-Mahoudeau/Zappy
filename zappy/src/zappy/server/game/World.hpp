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
#include "Event.hpp"
#include "IEntity.hpp"
#include "IEventEmitter.hpp"
#include "ResourceType.hpp"
#include "Tile.hpp"
#include "entity/Player.hpp"
#include "zappy/shared/io/Logger.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
class World : public IEventEmitter {
  public:
    struct Config {
        math::Vector2u size;
        std::uint16_t teamCount{};
        std::uint16_t playersPerTeam{};
        std::optional<io::Logger> logger;
    };

    static constexpr std::uint16_t kMajorTickInterval{20};

    explicit World(Config config);
    ~World() override = default;

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    World(World&&) = delete;
    World& operator=(World&&) = delete;

    /// @brief Updates the world state.
    void update();

    /// @brief Returns the size of the world.
    /// @return The size of the world.
    [[nodiscard]] math::Vector2u size() const;

    /// @brief Returns a reference to the entity database.
    /// @return A reference to the entity database.
    [[nodiscard]] const EntityDatabase& entityDatabase() const;

    /// @brief Returns a reference to the entity database.
    /// @return A reference to the entity database.
    [[nodiscard]] EntityDatabase& entityDatabase();

    /// @brief Returns a reference to the tile at the specified coordinates.
    /// @param pos The position of the tile.
    /// @return A reference to the tile at the specified coordinates.
    [[nodiscard]] const Tile& tile(math::Vector2u pos) const;

    /// @brief Returns a reference to the tile at the specified coordinates.
    /// @param position The position of the tile.
    /// @return A reference to the tile at the specified coordinates.
    [[nodiscard]] Tile& tile(math::Vector2u position);

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
    /// @param playerId The ID of the player to spawn the egg for.
    /// @param teamId The ID of the team to spawn the egg for.
    /// @return The ID of the spawned egg.
    [[nodiscard]] std::uint64_t spawnEgg(std::uint64_t playerId, std::uint16_t teamId);

    /// @brief Spawns an egg for the specified team.
    /// @param teamId The ID of the team to spawn the egg for.
    /// @return The ID of the spawned egg.
    [[nodiscard]] std::uint64_t spawnEgg(std::uint16_t teamId);

    /// @brief Spawns a resource of the specified type.
    /// @param type The type of resource to spawn.
    /// @return The ID of the spawned resource.
    void spawnResource(ResourceType type);

    /// @brief Hatches a random egg for the specified team.
    /// @param teamId The ID of the team to hatch the egg for.
    /// @return The ID of the new player, or an error message if no egg could be hatched.
    std::expected<std::uint64_t, std::string> hatchRandomEgg(std::uint16_t teamId);

    /// @brief Returns a view of all players in the specified team.
    /// @param teamId The ID of the team to get the players from.
    /// @return A view of all players in the specified team.
    [[nodiscard]] EntityDatabase::EntityView<const entity::Player> players(std::uint16_t teamId) const;

    /// @brief Returns a view of all players in the specified team.
    /// @param teamId The ID of the team to get the players from.
    /// @return A view of all players in the specified team.
    [[nodiscard]] EntityDatabase::EntityView<entity::Player> players(std::uint16_t teamId);

    /// @brief Returns the position of the specified entity.
    /// @param entityId The ID of the entity to get the position of.
    /// @return The position of the specified entity.
    [[nodiscard]] std::optional<math::Vector2u> position(std::uint64_t entityId) const;

    /// @brief Moves the specified entity to the specified position.
    /// @param entityId The ID of the entity to move.
    /// @param position The position to move the entity to.
    void moveTo(std::uint64_t entityId, math::Vector2u position);

    /// @brief Moves the specified entity by the specified delta.
    /// @param entityId The ID of the entity to move.
    /// @param delta The delta to move the entity by.
    /// @return The new position of the entity.
    [[nodiscard]] math::Vector2u moveBy(std::uint64_t entityId, math::Vector2i delta);

    /// @brief Returns true if the world has any events to process.
    /// @return True if the world has events, false otherwise.
    [[nodiscard]] bool hasEvents() const;

    /// @brief Returns the next event in the queue.
    /// @return The next event in the queue.
    [[nodiscard]] Event popEvent();

    /// @brief Adds an event to the queue.
    /// @param event The event to add.
    void pushEvent(Event event) override;

  private:
    /// @brief Returns the resource densities for the world.
    /// @return A map of resource types to their densities.
    [[nodiscard]] static const std::unordered_map<ResourceType, float>& resourceDensities();

    /// @brief Returns true if the specified coordinates are within the bounds of the world.
    /// @param position The position to check.
    /// @return True if the specified coordinates are within the bounds of the world, false otherwise.
    [[nodiscard]] bool isInBounds(math::Vector2u position) const;

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
    std::unordered_map<ResourceType, std::uint64_t> _resourceThresholds;
    std::uint16_t _nextMajorTick{kMajorTickInterval};
    std::vector<Event> _events;
};

template <IsEntity T>
std::uint64_t World::count() const {
    return _entityDatabase.countAll<T>();
}
}  // namespace zappy::server::game
