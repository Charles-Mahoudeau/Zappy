/*
** EPITECH PROJECT, 2026
** WorldInterationCommand
** File description:
** WorldInterationCommand header
*/

#pragma once

#include <cstdint>
#include <functional>

#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/server/game/World.hpp"
#include "zappy/server/game/entity/Player.hpp"
namespace zappy::server::command::player {

class WorldInterationCommand {
  public:
    static bool take(ICommandGroup::CommandCtx& ctx);
    static bool drop(ICommandGroup::CommandCtx& ctx);
    static bool eject(ICommandGroup::CommandCtx& ctx);

  private:
    static bool action(ICommandGroup::CommandCtx& ctx,
                       std::function<bool(game::ResourceType resource, game::entity::Player* player)>);

    static bool ejectPushPlayer(const game::entity::Player* pusher, game::entity::Player* pushed,
                                client::ClientRegistry& clientRegistry);
    static bool ejectDestroyEgg(game::World& world, std::uint64_t entityId);
};

}  // namespace zappy::server::command::player
