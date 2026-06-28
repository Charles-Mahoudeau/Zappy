/*
** EPITECH PROJECT, 2026
** WorldInterationCommand
** File description:
** WorldInterationCommand code
*/

#include "zappy/server/commands/player/WorldInterationCommand.hpp"

#include <cstdint>
#include <functional>
#include <utility>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/commands/player/PlayerData.hpp"
#include "zappy/server/game/Event.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/server/game/Tile.hpp"
#include "zappy/server/game/entity/Egg.hpp"
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

bool WorldInterationCommand::action(
    ICommandGroup::CommandCtx& ctx,
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

bool WorldInterationCommand::eject(ICommandGroup::CommandCtx& ctx) {
    auto clients = ctx.clientRegistry;
    auto world = ctx.world;
    auto id = ctx.client->playerID();

    ctx.client->setTimeout(7, [clients, world, id]() {
        const player::PlayerData data(clients, world, id);

        if (!data.valid()) {
            return;
        }
        const game::entity::Player* pusher = data.player();
        world.get().pushEvent(game::PlayerExpulsionEvent{.playerId = pusher->id()});
        const game::Tile& tile = world.get().grid().tile(data.player()->position());
        bool content = false;

        for (std::uint16_t entityId : tile.entities()) {
            auto* pushed = world.get().player(entityId);

            if (pushed != nullptr && pushed != pusher) {
                pushed->move(pusher->orientation());
                if (const Client* pushedClient = clients.get().findByPlayerId(pushed->id()); pushedClient != nullptr) {
                    std::ignore =
                        pushedClient->sendMessage("eject: {}\n", static_cast<std::uint8_t>(pusher->orientation()) + 1);
                }
                content = true;
                continue;
            }
            if (world.get().entityDatabase().is<game::entity::Egg>(entityId)) {
                world.get().remove(entityId);
                world.get().pushEvent(game::EggDeathEvent{.eggId = entityId});
                content = true;
            }
        }
        if (content) {
            data.client()->sendSuccess();
        } else {
            data.client()->sendError();
        }
    });
    return true;
}

}  // namespace zappy::server::command::player
