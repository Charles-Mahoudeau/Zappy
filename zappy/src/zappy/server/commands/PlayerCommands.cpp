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
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/server/game/entity/Egg.hpp"

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
          {"Take", [](auto& ctx) { return PlayerCommands::take(ctx); }},
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

bool PlayerCommands::inventory(CommandCtx& ctx) {
    ctx.client->setTimeout(1, [&ctx, id = ctx.client->playerID()]() {
        const player::PlayerData data(ctx, id);

        if (!data.valid()) {
            return;
        }
        std::ignore = ctx.client->sendMessage("[ {} ]\n", data.player()->inventory().detailledString());
    });
    return true;
}

bool PlayerCommands::take(CommandCtx& ctx) {
    ctx.client->setTimeout(7, [&ctx, id = ctx.client->playerID(), params = ctx.data.params]() {
        player::PlayerData data(ctx, id);

        if (!data.valid()) {
            return;
        }
        if (params.size() != 1) {
            data.client()->sendError();
            return;
        }
        auto resource = game::ResourceHelper::strToRessource(params.at(0));

        if (!resource.has_value() || !ctx.world.get().playerTake(data.player(), resource.value())) {
            data.client()->sendError();
        } else {
            data.client()->sendSuccess();
        }
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

    auto eggs =
        std::views::filter(ctx.world.get().entityDatabase().viewAll<game::entity::Egg>(),
                           [teamName = team->name()](game::entity::Egg* egg) { return egg->teamName() == teamName; });
    std::ignore = client->sendMessage("{}\n", std::ranges::distance(eggs));
    return true;
}

}  // namespace zappy::server::command
