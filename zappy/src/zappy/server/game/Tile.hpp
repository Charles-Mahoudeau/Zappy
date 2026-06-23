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
#include "Inventory.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
class World;

class Tile {
  public:
    Tile(World& world, math::Vector2u position);
    ~Tile() = default;

    Tile(const Tile&) = default;
    Tile& operator=(const Tile&) = default;

    Tile(Tile&&) = default;
    Tile& operator=(Tile&&) = default;

    /// @brief Returns the position of the tile.
    /// @return The position of the tile.
    [[nodiscard]] math::Vector2u position() const;

    /// @brief Returns the number of entities in the tile.
    /// @return The number of entities in the tile.
    [[nodiscard]] std::uint64_t count() const;

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

    /// @brief Returns the inventory of the tile.
    /// @return The inventory of the tile.
    [[nodiscard]] const Inventory& inventory() const;

    /// @brief Returns a reference to the inventory of the tile.
    /// @return A reference to the inventory of the tile.
    Inventory& inventory();

  private:
    [[nodiscard]] const EntityDatabase& entityDatabase() const;
    [[nodiscard]] EntityDatabase& entityDatabase();

    std::reference_wrapper<World> _world;
    math::Vector2u _position;
    std::vector<std::uint64_t> _entities;
    Inventory _inventory;
};

template <IsEntity T>
std::uint64_t Tile::count() const {
    return entityDatabase().filter<T>(_entities).size();
}
}  // namespace zappy::server::game
