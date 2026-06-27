/*
** EPITECH PROJECT, 2026
** PlayerCommands
** File description:
** PlayerCommands code
*/

#include "zappy/server/commands/PlayerCommands.hpp"

#include <cstdint>
#include <optional>
#include <string_view>
#include <utility>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/World.hpp"

namespace zappy::server::command {
PlayerCommands::PlayerCommands(CommandCtx context)
    : ACommandGroup{std::move(context)},
      _commands({
          {"Forward", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Right", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Left", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Look", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Inventory", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Broadcast", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Connect_nbr", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Fork", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Eject", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Take", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Set", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Incantation", [](const CommandCtx& ctx) { return incantation(ctx); }},
      }) {}

void PlayerCommands::execute(Client* client, [[maybe_unused]] const std::string_view msg) {
    CommandCtx& ctx = this->commandCtx();

    if (const auto iter = this->_commands.find(ctx.data.name); iter != this->_commands.end()) {
        if (!iter->second(ctx)) {
            (void)client->sendMessage("ko\n");
        }
    } else {
        (void)client->sendMessage("ko\n");
    }
}

bool PlayerCommands::ignore(const CommandCtx& ctx) {
    (void)ctx;
    return false;
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

    const auto broadcastMessage = [&clientRegistry = ctx.clientRegistry.get(), &logger = ctx.logger.get()](
                                      const game::World::IncantationSnapshot& incantationSnapshot,
                                      const std::string_view message) {
        for (const std::uint64_t playerId : incantationSnapshot.playerIds) {
            const Client* client = clientRegistry.findByPlayerId(playerId);

            if (client == nullptr) {
                logger.warn("Client associated with player ID {} not found", playerId);
                continue;
            }
            std::ignore = client->sendMessage(message);
        }
    };

    ctx.timer.get().scheduleLater(kIncantationTimeLimit, [&world = ctx.world.get(), &logger = ctx.logger.get(),
                                                          snapshot = *snapshot, broadcastMessage] {
        if (!world.endIncantation(snapshot)) {
            broadcastMessage(snapshot, "ko\n");
            return;
        }
        broadcastMessage(snapshot, std::format("Current level: {}\n", snapshot.level + 1));
        logger.info("Incantation started by player #{} has reached level {}.", snapshot.playerId, snapshot.level + 1);
    });
    ctx.client->setTimeout(kIncantationTimeLimit);
    broadcastMessage(*snapshot, "Elevation underway\n");
    return true;
}
}  // namespace zappy::server::command
