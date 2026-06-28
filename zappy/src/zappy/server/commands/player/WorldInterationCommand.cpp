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
#include "zappy/server/client/ClientRegistry.hpp"
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
        bool content = false;
        const game::entity::Player* pusher = data.player();

        world.get().pushEvent(game::PlayerExpulsionEvent{.playerId = pusher->id()});

        for (std::uint16_t entityId : world.get().grid().tile(data.player()->position()).entities()) {
            if (WorldInterationCommand::ejectPushPlayer(pusher, world.get().player(entityId), clients) ||
                WorldInterationCommand::ejectDestroyEgg(world, entityId)) {
                content = true;
            }
        }
        content ? data.client()->sendSuccess() : data.client()->sendError();
    });
    return true;
}

bool WorldInterationCommand::ejectPushPlayer(const game::entity::Player* pusher, game::entity::Player* pushed,
                                             client::ClientRegistry& clientRegistry) {
    if (pushed != nullptr && pushed != pusher) {
        if (const Client* pushedClient = clientRegistry.findByPlayerId(pushed->id()); pushedClient != nullptr) {
            pushed->move(pusher->orientation());
            std::ignore = pushedClient->sendMessage("eject: {}\n", std::to_underlying(pusher->orientation()) + 1);
            return true;
        }
    }
    return false;
}
bool WorldInterationCommand::ejectDestroyEgg(game::World& world, std::uint64_t entityId) {
    if (world.entityDatabase().is<game::entity::Egg>(entityId)) {
        world.remove(entityId);
        world.pushEvent(game::EggDeathEvent{.eggId = entityId});
        return true;
    }
    return false;
}

}  // namespace zappy::server::command::player
