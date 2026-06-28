/*
** EPITECH PROJECT, 2026
** WorldInterationCommand
** File description:
** WorldInterationCommand code
*/

#include <functional>
#include <utility>

#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/commands/player/WorldInterationCommand.hpp"
#include "zappy/server/commands/player/PlayerData.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/server/game/entity/Player.hpp"

namespace zappy::server::command::player {

bool WorldInterationCommand::take(ICommandGroup::CommandCtx& ctx) {
    return WorldInterationCommand::action(ctx, [&ctx](game::ResourceType resource, game::entity::Player* player) {
        return ctx.world.get().playerTake(player, resource);
    });
}

bool WorldInterationCommand::drop(ICommandGroup::CommandCtx& ctx) {
    return WorldInterationCommand::action(ctx, [&ctx](game::ResourceType resource, game::entity::Player* player) {
        return ctx.world.get().playerDrop(player, resource);
    });
}

bool WorldInterationCommand::action(ICommandGroup::CommandCtx& ctx,
                           std::function<bool(game::ResourceType resource, game::entity::Player* player)> event) {
    auto clients = ctx.clientRegistry;
    auto world = ctx.world;
    auto id = ctx.client->playerID();

    ctx.client->setTimeout(7, [clients, world, id, params = std::move(ctx.data.params), event = std::move(event)]() {
        player::PlayerData data(clients, world, id);

        if (!data.valid()) {
            return;
        }
        if (params.size() != 1) {
            data.client()->sendError();
            return;
        }
        auto resource = game::ResourceHelper::strToRessource(params.at(0));

        if (!resource.has_value() || !event(resource.value(), data.player())) {
            data.client()->sendError();
        } else {
            data.client()->sendSuccess();
        }
    });
    return true;
}

}  // namespace zappy::server::command::player
