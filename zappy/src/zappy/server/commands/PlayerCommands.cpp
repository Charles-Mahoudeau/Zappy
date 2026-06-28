/*
** EPITECH PROJECT, 2026
** PlayerCommands
** File description:
** PlayerCommands code
*/

#include "zappy/server/commands/PlayerCommands.hpp"

#include <cstdint>
#include <format>
#include <functional>
#include <iterator>
#include <optional>
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
#include "zappy/server/commands/player/PlayerData.hpp"
#include "zappy/server/commands/player/WorldInterationCommand.hpp"
#include "zappy/server/game/Event.hpp"
#include "zappy/server/game/Tile.hpp"
#include "zappy/server/game/World.hpp"
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
          {"Eject", [](auto& ctx) { return player::WorldInterationCommand::eject(ctx); }},
          {"Take", [](auto& ctx) { return player::WorldInterationCommand::take(ctx); }},
          {"Set", [](auto& ctx) { return player::WorldInterationCommand::drop(ctx); }},
          {"Incantation", [](const CommandCtx& ctx) { return incantation(ctx); }},
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

bool PlayerCommands::inventory(CommandCtx& ctx) {
    auto clients = ctx.clientRegistry;
    auto world = ctx.world;
    auto id = ctx.client->playerID();

    ctx.client->setTimeout(1, [clients, world, id]() {
        const player::PlayerData data(clients, world, id);

        if (!data.valid()) {
            return;
        }
        std::ignore = data.client()->sendMessage("[{}]\n", data.player()->inventory().detailedString());
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
            if (const game::entity::Player* receiver = world.get().player(client->playerID()); receiver != nullptr) {
                std::ignore = client->sendMessage(
                    "message {}, {}\n", world.get().computeDistFromPositions(emitter->position(), receiver->position()),
                    text);
            }
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
            return;
        }
        data.client()->sendSuccess();
    });
    return true;
}

bool PlayerCommands::look(CommandCtx& ctx) {
    auto clients = ctx.clientRegistry;
    auto world = ctx.world;
    auto id = ctx.client->playerID();

    ctx.client->setTimeout(7, [clients, world, id]() {
        const player::PlayerData data(clients, world, id);

        if (!data.valid()) {
            return;
        }
        auto tiles = world.get().playerView(data.player());

        std::string msg;
        for (const auto& tile : tiles) {
            msg += tile.get().string(world.get().entityDatabase()) + ",";
        }
        std::ignore = data.client()->sendMessage("[{}]\n", msg);
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

bool PlayerCommands::incantation(const CommandCtx& ctx) {
    if (ctx.client == nullptr) {
        return false;
    }

    const std::optional<game::World::IncantationSnapshot> snapshot =
        ctx.world.get().beginIncantation(ctx.client->playerID());

    if (!snapshot.has_value()) {
        return false;
    }

    const auto broadcastMessage = [&clientRegistry = ctx.clientRegistry.get(), &logger = ctx.logger.get(),
                                   timeLimit = kIncantationTimeLimit](
                                      const game::World::IncantationSnapshot& incantationSnapshot,
                                      const std::string_view message, const bool freeze) {
        for (const std::uint64_t playerId : incantationSnapshot.playerIds) {
            Client* client = clientRegistry.findByPlayerId(playerId);

            if (client == nullptr) {
                logger.warn("Client associated with player ID {} not found", playerId);
                continue;
            }
            std::ignore = client->sendMessage(message);
            if (freeze) {
                client->setTimeout(timeLimit);
            }
        }
    };

    ctx.timer.get().scheduleLater(kIncantationTimeLimit, [&world = ctx.world.get(), &logger = ctx.logger.get(),
                                                          snapshot = *snapshot, broadcastMessage] {
        if (!world.endIncantation(snapshot)) {
            broadcastMessage(snapshot, "ko\n", false);
            return;
        }
        broadcastMessage(snapshot, std::format("Current level: {}\n", snapshot.level + 1), false);
        logger.info("Incantation started by player #{} has reached level {}.", snapshot.playerId, snapshot.level + 1);
    });
    broadcastMessage(*snapshot, "Elevation underway\n", true);
    return true;
}
}  // namespace zappy::server::command
