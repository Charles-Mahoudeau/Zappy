/*
** EPITECH PROJECT, 2026
** PlayerCommands
** File description:
** PlayerCommands code
*/

#include "zappy/server/commands/PlayerCommands.hpp"

#include <cstdint>
#include <print>
#include <string_view>
#include <tuple>
#include <utility>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/IEntity.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/server/game/entity/Player.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server::command {
PlayerCommands::PlayerCommands(CommandCtx context)
    : ACommandGroup{std::move(context)},
      _commands({
          {"Forward", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Right", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Left", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Look", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Inventory",
           [](auto& ctx) {
               PlayerCommands::inventory(ctx);
               return true;
           }},
          {"Broadcast", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Connect_nbr", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Fork", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Eject", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Take", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
          {"Set", [](auto& ctx) { return PlayerCommands::ignore(ctx); }},
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

void PlayerCommands::inventory(CommandCtx& ctx) {
    ctx.client->setTimeout(1, [&ctx, id = ctx.client->playerID()]() {
        const PlayerData data = PlayerCommands::idToPlayerData(ctx, id);

        if (data.client == nullptr) {
            return;
        }
        if (data.player == nullptr) {
            data.client->sendError();
        } else {
            std::ignore = ctx.client->sendMessage("[ {} ]\n", data.player->inventory().detailledString());
        }
    });
}

PlayerCommands::PlayerData PlayerCommands::idToPlayerData(CommandCtx& ctx, std::uint64_t id) {
    PlayerData data;
    data.client = ctx.clientRegistry.get().findByID(id);
    game::IEntity* entity = ctx.world.get().entityDatabase().query(id);
    if (entity == nullptr) {
        return data;
    }

    data.player = dynamic_cast<game::entity::Player*>(entity);
    return data;
}

}  // namespace zappy::server::command
