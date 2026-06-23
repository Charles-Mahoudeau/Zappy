/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Direction
*/

#pragma once

#include <cstdint>

namespace zappy::math {
enum class Direction : std::uint8_t {
    kNorth = 0,
    kSouth = 1,
    kEast = 2,
    kWest = 3,
};

namespace direction {
/// @brief Rotates the given direction 90 degrees counter-clockwise (left).
/// @param direction The current orientation to rotate.
/// @return The new orientation facing left of the input.
Direction turnLeft(Direction direction);

/// @brief Rotates the given direction 90 degrees clockwise (right).
/// @param direction The current orientation to rotate.
/// @return The new orientation facing right of the input.
Direction turnRight(Direction direction);
}  // namespace direction
}  // namespace zappy::math
