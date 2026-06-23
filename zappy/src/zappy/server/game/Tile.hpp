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
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
class World;

class Tile {
  public:
    explicit Tile(math::Vector2u position);
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
    math::Vector2u _position;
    std::vector<std::uint64_t> _entities;
};
}  // namespace zappy::server::game
