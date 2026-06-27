/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** World
*/

#pragma once

#include <cstdint>
#include <deque>
#include <expected>
#include <optional>
#include <random>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>

#include "EntityDatabase.hpp"
#include "Event.hpp"
#include "Grid.hpp"
#include "IEntity.hpp"
#include "IEventEmitter.hpp"
#include "ResourceType.hpp"
#include "Tile.hpp"
#include "entity/Player.hpp"
#include "zappy/server/Timer.hpp"
#include "zappy/shared/io/Logger.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
class World : public IEventEmitter {
  public:
    static constexpr std::uint16_t kFoodRegenerationInterval{20};

    explicit World(math::Vector2u size, Timer& timer, std::optional<io::Logger> logger = std::nullopt);
    ~World() override = default;

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    World(World&&) = delete;
    World& operator=(World&&) = delete;

    /// @brief Returns the size of the world.
    /// @return The size of the world.
    [[nodiscard]] math::Vector2u size() const;

    /// @brief Returns a reference to the entity database.
    /// @return A reference to the entity database.
    [[nodiscard]] const EntityDatabase& entityDatabase() const;

    /// @brief Returns a reference to the grid.
    /// @return A reference to the grid.
    [[nodiscard]] const Grid& grid() const;

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
    /// @param teamName The ID of the team to spawn the egg for.
    /// @return The ID of the spawned egg.
    [[nodiscard]] std::uint64_t spawnEgg(std::uint64_t playerId, std::string_view teamName);

    /// @brief Spawns an egg for the specified team.
    /// @param teamName The ID of the team to spawn the egg for.
    /// @return The ID of the spawned egg.
    [[nodiscard]] std::uint64_t spawnEgg(std::string_view teamName);

    /// @brief Spawns a resource of the specified type.
    /// @param type The type of resource to spawn.
    /// @return The ID of the spawned resource.
    void spawnResource(ResourceType type);

    /// @brief Spawns the initial eggs in the world.
    void spawnStartEggs(std::span<const std::string_view> teams, std::uint8_t playersPerTeam);

    /// @brief Spawns the initial eggs in the world.
    void spawnStartEggs(std::span<const std::string> teams, std::uint8_t playersPerTeam);

    /// @brief Hatches a random egg for the specified team.
    /// @param teamName The name of the team to hatch the egg for.
    /// @return The ID of the new player, or an error message if no egg could be hatched.
    std::expected<std::uint64_t, std::string> hatchRandomEgg(std::string_view teamName);

    /// @brief get the number of Egg in the given team
    /// @param teamName The name of the team to hatch the egg for.
    /// @return The number of egg in the team
    [[nodiscard]] std::uint64_t eggCount(std::string_view teamName) const;

    /// @brief Returns a view of all players in the specified team.
    /// @param teamName The name of the team to get the players from.
    /// @return A view of all players in the specified team.
    [[nodiscard]] EntityDatabase::EntityView<const entity::Player> players(std::string_view teamName) const;

    /// @brief Returns a view of all players in the specified team.
    /// @param teamName The name of the team to get the players from.
    /// @return A view of all players in the specified team.
    [[nodiscard]] EntityDatabase::EntityView<entity::Player> players(std::string_view teamName);

    /// @brief Returns a view of the player with the given ID.
    /// @param id The ID of the player to get.
    /// @return A pointer to the player with the given ID, or nullptr if no such player exists.
    [[nodiscard]] const entity::Player* player(std::uint64_t id) const;

    /// @brief Returns a view of the player with the given ID.
    /// @param id The ID of the player to get.
    /// @return A pointer to the player with the given ID, or nullptr if no such player exists.
    [[nodiscard]] entity::Player* player(std::uint64_t id);

    /// @brief Removes the specified entity from the world.
    /// @param entityId The ID of the entity to remove.
    void remove(std::uint64_t entityId);

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

    /// @brief Spawns resources in the world to meet the threshold.
    void spawnResources();

    /// @brief Returns a random tile in the world.
    /// @return A reference to a random tile in the world.
    [[nodiscard]] Tile& randomTile();

    /// @brief Generates the resource thresholds for the world.
    void generateResourceThresholds();

    /// @brief Places an egg at a random tile in the world.
    /// @param eggId The ID of the egg to place.
    void placeEggRandom(std::uint64_t eggId);

    std::random_device _randomDevice;
    std::mt19937 _randomEngine{_randomDevice()};
    EntityDatabase _entityDatabase;
    Grid _grid;
    std::reference_wrapper<Timer> _timer;
    std::optional<io::Logger> _logger;
    std::unordered_map<ResourceType, std::uint64_t> _resourceThresholds;
    std::deque<Event> _events;
};

template <IsEntity T>
std::uint64_t World::count() const {
    return _entityDatabase.countAll<T>();
}
}  // namespace zappy::server::game
