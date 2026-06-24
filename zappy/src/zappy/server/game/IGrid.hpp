/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** IGrid
*/

#pragma once

#include <cstdint>
#include <optional>

#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
class IGrid {
  public:
    IGrid() = default;
    virtual ~IGrid() = default;

    IGrid(const IGrid&) = default;
    IGrid& operator=(const IGrid&) = default;

    IGrid(IGrid&&) = default;
    IGrid& operator=(IGrid&&) = default;

    /// @brief Returns the size of the grid.
    /// @return The size of the grid.
    [[nodiscard]] virtual math::Vector2u size() const = 0;

    /// @brief Returns the position of the specified entity.
    /// @param entityId The ID of the entity to get the position of.
    /// @return The position of the specified entity.
    [[nodiscard]] virtual std::optional<math::Vector2u> position(std::uint64_t entityId) const = 0;

    /// @brief Sets the position of the specified entity.
    /// @param entityId The ID of the entity to set the position of.
    /// @param position The new position of the entity.
    virtual void setPosition(std::uint64_t entityId, math::Vector2u position) = 0;

    /// @brief Removes the specified entity from the grid.
    /// @param entityId The ID of the entity to remove.
    virtual void remove(std::uint64_t entityId) = 0;
};
}  // namespace zappy::server::game
