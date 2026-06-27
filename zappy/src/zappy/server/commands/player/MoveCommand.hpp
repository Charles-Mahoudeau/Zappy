/*
** EPITECH PROJECT, 2026
** MoveCommand
** File description:
** MoveCommand header
*/

#pragma once

#include <cstdint>

#include "zappy/server/commands/ICommandGroup.hpp"
namespace zappy::server::command::player {

class MoveCommand {
  public:
    static bool forward(ICommandGroup::CommandCtx& ctx);
    static bool right(ICommandGroup::CommandCtx& ctx);
    static bool left(ICommandGroup::CommandCtx& ctx);

  private:
    enum class Move : std::uint8_t {
        kRight,
        kLeft,
        kForward,
    };

    static bool move(ICommandGroup::CommandCtx& ctx, Move move);
    // Members
};

}  // namespace zappy::server::command::player
