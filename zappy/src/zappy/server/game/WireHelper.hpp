/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** WireHelper
*/

#pragma once

#include <string>

#include "Tile.hpp"
#include "World.hpp"

namespace zappy::server::game {
class WireHelper {
  public:
    /// @brief Converts a tile to a bct command.
    /// @param tile The tile to convert.
    /// @return The bct command.
    [[nodiscard]] static std::string tileToBctCommand(const Tile& tile);

    /// @brief Converts a world to a bct commands.
    /// @param world The world to convert.
    /// @return The bct commands.
    [[nodiscard]] static std::string worldToBctCommands(const World& world);
};
}  // namespace zappy::server::game
