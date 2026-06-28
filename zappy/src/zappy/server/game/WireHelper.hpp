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
    /// @return The commands.
    [[nodiscard]] static std::string tileToCommands(const Tile& tile);

    /// @brief Converts a world to tile commands.
    /// @param world The world to convert.
    /// @return The tile commands.
    [[nodiscard]] static std::string worldToTileCommands(const World& world);

    /// @brief Converts a world to egg commands.
    /// @param world The world to convert.
    /// @return The egg commands.
    static std::string worldToEggCommands(const World& world);

    /// @brief Converts a world to player commands.
    /// @return The player commands.
    static std::string worldToPlayerCommands(const World& world);
};
}  // namespace zappy::server::game
