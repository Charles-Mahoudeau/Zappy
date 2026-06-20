/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Tile
*/

#pragma once

#include <cstdint>
#include <functional>
#include <span>
#include <vector>

#include "EntityDatabase.hpp"
#include "IEntity.hpp"

namespace zappy::server::game {
class World;

class Tile {
  public:
    explicit Tile(World& world, std::uint16_t x, std::uint16_t y);
    ~Tile() = default;

    Tile(const Tile&) = default;
    Tile& operator=(const Tile&) = default;

    Tile(Tile&&) = default;
    Tile& operator=(Tile&&) = default;

    /// @brief Returns the x-coordinate of the tile.
    /// @return The x-coordinate of the tile.
    [[nodiscard]] std::uint16_t x() const;

    /// @brief Returns the y-coordinate of the tile.
    /// @return The y-coordinate of the tile.
    [[nodiscard]] std::uint16_t y() const;

    /// @brief Returns the number of entities in the tile.
    /// @return The number of entities in the tile.
    [[nodiscard]] std::uint16_t count() const;

    /// @brief Returns the number of entities of the specified type in the tile.
    /// @tparam T The type of entity to count.
    /// @return The number of entities of the specified type in the tile.
    template <IsEntity T>
    [[nodiscard]] std::uint64_t count() const;

    /// @brief Returns the entities in the tile.
    /// @return The entities in the tile.
    [[nodiscard]] std::span<const std::uint64_t> entities() const;

    /// @brief Returns true if the tile has the specified entity.
    /// @param entityId The ID of the entity to check.
    /// @return True if the tile has the specified entity, false otherwise.
    [[nodiscard]] bool hasEntity(std::uint64_t entityId) const;

    /// @brief Adds an entity to the tile.
    /// @param entityId The ID of the entity to add.
    void addEntity(std::uint64_t entityId);

    /// @brief Removes an entity from the tile.
    /// @param entityId The ID of the entity to remove.
    /// @return True if the entity was removed, false otherwise.
    bool removeEntity(std::uint64_t entityId);

  private:
    [[nodiscard]] const EntityDatabase& entityDatabase() const;
    [[nodiscard]] EntityDatabase& entityDatabase();

    std::reference_wrapper<World> _world;
    std::uint16_t _x;
    std::uint16_t _y;
    std::vector<std::uint64_t> _entities;
};

template <IsEntity T>
std::uint64_t Tile::count() const {
    return entityDatabase().filter<T>(_entities).size();
}
}  // namespace zappy::server::game
