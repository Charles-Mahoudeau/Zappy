/*
** EPITECH PROJECT, 2026
** ObjectCommand
** File description:
** ObjectCommand header
*/

#pragma once

#include <functional>

#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/server/game/entity/Player.hpp"
namespace zappy::server::command::player {

class ObjectCommand {
  public:
    static bool take(ICommandGroup::CommandCtx& ctx);
    static bool drop(ICommandGroup::CommandCtx& ctx);

  private:
    static bool action(ICommandGroup::CommandCtx& ctx,
                       std::function<bool(game::ResourceType resource, game::entity::Player* player)>);
};

}  // namespace zappy::server::command::player
