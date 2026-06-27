/*
** EPITECH PROJECT, 2026
** PlayerCommands
** File description:
** PlayerCommands code
*/

#include "zappy/server/commands/PlayerCommands.hpp"

#include <functional>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <utility>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/Team.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/commands/player/MoveCommand.hpp"
#include "zappy/server/commands/player/ObjectCommand.hpp"
#include "zappy/server/commands/player/PlayerData.hpp"
#include "zappy/server/game/Event.hpp"
#include "zappy/server/game/entity/Egg.hpp"
#include "zappy/server/game/entity/Player.hpp"
#include "zappy/shared/exception/Exception.hpp"

namespace zappy::server::command {
PlayerCommands::PlayerCommands(CommandCtx context)
    : ACommandGroup{std::move(context)},
      _commands({
          {"Forward", [](auto& ctx) { return player::MoveCommand::forward(ctx); }},
          {"Right", [](auto& ctx) { return player::MoveCommand::right(ctx); }},
          {"Left", [](auto& ctx) { return player::MoveCommand::left(ctx); }},
          {"Look", [](auto& ctx) { return PlayerCommands::look(ctx); }},
          {"Inventory", [](auto& ctx) { return PlayerCommands::inventory(ctx); }},
          {"Broadcast", [](auto& ctx) { return PlayerCommands::broadcast(ctx); }},
          {"Connect_nbr", [](auto& ctx) { return PlayerCommands::connectNb(ctx); }},
          {"Fork", [](auto& ctx) { return PlayerCommands::fork(ctx); }},
          {"Eject", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Take", [](auto& ctx) { return player::ObjectCommand::take(ctx); }},
          {"Set", [](auto& ctx) { return player::ObjectCommand::drop(ctx); }},
          {"Incantation", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
      }) {}

void PlayerCommands::execute(Client* client, [[maybe_unused]] const std::string_view msg) {
    CommandCtx& ctx = this->commandCtx();

    if (const auto iter = this->_commands.find(ctx.data.name); iter != this->_commands.end()) {
        if (!iter->second(ctx)) {
            client->sendError();
        }
    } else {
        client->sendError();
    }
}

bool PlayerCommands::ignore(const CommandCtx& ctx) {
    (void)ctx;
    return false;
}

bool PlayerCommands::inventory(CommandCtx& ctx) {
    auto clients = ctx.clientRegistry;
    auto world = ctx.world;
    auto id = ctx.client->playerID();

    ctx.client->setTimeout(1, [clients, world, id]() {
        const player::PlayerData data(clients, world, id);

        if (!data.valid()) {
            return;
        }
        std::ignore = data.client()->sendMessage("[ {} ]\n", data.player()->inventory().detailedString());
    });
    return true;
}

bool PlayerCommands::broadcast(CommandCtx& ctx) {
    auto clients = ctx.clientRegistry;
    auto world = ctx.world;
    auto id = ctx.client->playerID();
    std::string text;

    for (const auto& word : ctx.data.params) {
        text += " " + word;
    }

    ctx.client->setTimeout(7, [clients, world, id, text = std::move(text)]() {
        const player::PlayerData data(clients, world, id);

        if (!data.valid()) {
            return;
        }
        const game::entity::Player* emitter = data.player();
        world.get().pushEvent(game::PlayerBroadcastEvent{.playerId = id, .message = text});
        for (const auto& client : clients.get().viewAll(Client::Type::kPlayer)) {
            if (client->playerID() == id) {
                continue;
            }
            const game::entity::Player* receiver = world.get().player(client->playerID());
            if (receiver == nullptr) {
                continue;
            }
            std::ignore = client->sendMessage(
                "message {},{}\n", world.get().computeDistFromPositions(emitter->position(), receiver->position()),
                text);
        }
        data.client()->sendSuccess();
    });
    return true;
}

bool PlayerCommands::fork(CommandCtx& ctx) {
    auto clients = ctx.clientRegistry;
    auto world = ctx.world;
    auto id = ctx.client->playerID();
    auto logger = ctx.logger;

    ctx.client->setTimeout(42, [clients, world, id, logger]() {
        const player::PlayerData data(clients, world, id);

        if (!data.valid()) {
            return;
        }

        try {
            std::ignore = world.get().spawnEgg(id, data.player()->teamName());
        } catch (const exception::Exception& err) {
            logger.get().error(err.what());
            data.client()->sendError();
        }
        data.client()->sendSuccess();
    });
    return true;
}

bool PlayerCommands::look(CommandCtx& ctx) {
    auto clients = ctx.clientRegistry;
    auto world = ctx.world;
    auto id = ctx.client->playerID();
    auto logger = ctx.logger;

    ctx.client->setTimeout(1, [clients, world, id, logger]() {
        const player::PlayerData data(clients, world, id);

        if (!data.valid()) {
            return;
        }
        auto tiles = world.get().playerView(data.player());

        std::string msg = "[";

        for (const auto& tile : tiles) {
            msg += tile.get().string(world.get().entityDatabase()) + ",";
        }
        msg += "]";
        std::ignore = data.client()->sendMessage("{}\n", msg);
    });
    return true;
}
bool PlayerCommands::connectNb(CommandCtx& ctx) {
    auto* client = ctx.client;
    const client::Team* team = ctx.teamRegistry.get().team(client->address());
    if (team == nullptr) {
        std::ignore = client->sendMessage("{}\n", 0);
        return true;
    }

    auto eggs = std::ranges::filter_view(
        ctx.world.get().entityDatabase().viewAll<game::entity::Egg>(),
        [teamName = team->name()](const game::entity::Egg* egg) { return egg->teamName() == teamName; });
    std::ignore = client->sendMessage("{}\n", std::ranges::distance(eggs));
    return true;
}

}  // namespace zappy::server::command
