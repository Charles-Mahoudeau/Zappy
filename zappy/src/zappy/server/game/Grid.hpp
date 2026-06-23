/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Grid
*/

#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <vector>

#include "IGrid.hpp"
#include "Tile.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
class Grid : public IGrid {
  public:
    explicit Grid(math::Vector2u size);
    ~Grid() override = default;

    Grid(const Grid&) = default;
    Grid& operator=(const Grid&) = default;

    Grid(Grid&&) = default;
    Grid& operator=(Grid&&) = default;

    /// @brief Returns the size of the grid.
    /// @return The size of the grid.
    [[nodiscard]] math::Vector2u size() const override;

    /// @brief Returns the position of the specified entity.
    /// @param entityId The ID of the entity to get the position of.
    /// @return The position of the specified entity.
    /// @see IEntity::position()
    [[nodiscard]] std::optional<math::Vector2u> position(std::uint64_t entityId) const override;

    /// @brief Sets the position of the specified entity.
    /// @param entityId The ID of the entity to set the position of.
    /// @param position The new position of the entity.
    /// @see IEntity::setPosition()
    void setPosition(std::uint64_t entityId, math::Vector2u position) override;

    /// @brief Removes the specified entity from the world.
    /// @param entityId The ID of the entity to remove.
    void remove(std::uint64_t entityId) override;

    /// @brief Returns a reference to the tiles.
    /// @return A reference to the tiles.
    [[nodiscard]] std::span<const Tile> tiles() const;

    /// @brief Returns a reference to the tiles.
    /// @return A reference to the tiles.
    [[nodiscard]] std::span<Tile> tiles();

    /// @brief Returns a reference to the tile at the specified coordinates.
    /// @param position The position of the tile.
    /// @return A reference to the tile at the specified coordinates.
    [[nodiscard]] const Tile& tile(math::Vector2u position) const;

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

  private:
    /// @brief Returns true if the specified coordinates are within the bounds of the world.
    /// @param position The position to check.
    /// @return True if the specified coordinates are within the bounds of the world, false otherwise.
    [[nodiscard]] bool isInBounds(math::Vector2u position) const;

    std::vector<Tile> _tiles;
    math::Vector2u _size;
};
}  // namespace zappy::server::game
