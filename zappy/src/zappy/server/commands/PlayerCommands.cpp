/*
** EPITECH PROJECT, 2026
** PlayerCommands
** File description:
** PlayerCommands code
*/

#include "zappy/server/commands/PlayerCommands.hpp"

#include <functional>
#include <iterator>
#include <ranges>
#include <cstdint>
#include <format>
#include <optional>
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
#include "zappy/server/game/entity/Egg.hpp"
#include "zappy/server/game/World.hpp"

namespace zappy::server::command {
PlayerCommands::PlayerCommands(CommandCtx context)
    : ACommandGroup{std::move(context)},
      _commands({
          {"Forward", [](auto& ctx) { return player::MoveCommand::forward(ctx); }},
          {"Right", [](auto& ctx) { return player::MoveCommand::right(ctx); }},
          {"Left", [](auto& ctx) { return player::MoveCommand::left(ctx); }},
          {"Look", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Inventory", [](auto& ctx) { return PlayerCommands::inventory(ctx); }},
          {"Broadcast", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Connect_nbr", [](auto& ctx) { return PlayerCommands::connectNb(ctx); }},
          {"Fork", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Eject", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Take", [](auto& ctx) { return player::ObjectCommand::take(ctx); }},
          {"Set", [](auto& ctx) { return player::ObjectCommand::drop(ctx); }},
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
