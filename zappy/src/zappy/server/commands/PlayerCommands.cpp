/*
** EPITECH PROJECT, 2026
** PlayerCommands
** File description:
** PlayerCommands code
*/

#include "zappy/server/commands/PlayerCommands.hpp"

#include <cstdint>
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
#include "zappy/server/commands/player/PlayerData.hpp"
#include "zappy/server/game/IEntity.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/server/game/entity/Egg.hpp"
#include "zappy/server/game/entity/Player.hpp"

namespace zappy::server::command {
PlayerCommands::PlayerCommands(CommandCtx context)
    : ACommandGroup{std::move(context)},
      _commands({
          {"Forward", [](auto& ctx) { return PlayerCommands::move(ctx, Move::kForward); }},
          {"Right", [](auto& ctx) { return PlayerCommands::move(ctx, Move::kRight); }},
          {"Left", [](auto& ctx) { return PlayerCommands::move(ctx, Move::kLeft); }},
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
        const auto [player, client] = PlayerCommands::getclientData(ctx, id);

        if (client == nullptr || player == nullptr) {
            return;
        }
        std::ignore = ctx.client->sendMessage("[ {} ]\n", player->inventory().detailledString());
    });
    return true;
}

bool PlayerCommands::take(CommandCtx& ctx) {
    ctx.client->setTimeout(7, [&ctx, id = ctx.client->playerID(), params = ctx.data.params]() {
        const auto [player, client] = PlayerCommands::getclientData(ctx, id);

        if (client == nullptr || player == nullptr) {
            return;
        }
        if (params.size() != 1) {
            client->sendError();
            return;
        }
        auto resource = game::ResourceHelper::strToRessource(params.at(0));

        if (!resource.has_value() || !ctx.world.get().playerTake(player, resource.value())) {
            client->sendError();
        } else {
            client->sendSuccess();
        }
    });
    return true;
}

bool PlayerCommands::set(CommandCtx& ctx) {
    ctx.client->setTimeout(7, [&ctx, id = ctx.client->playerID(), params = ctx.data.params]() {
        const auto [player, client] = PlayerCommands::getclientData(ctx, id);

        if (client == nullptr || player == nullptr) {
            return;
        }
        if (params.size() != 1) {
            client->sendError();
            return;
        }
        auto resource = game::ResourceHelper::strToRessource(params.at(0));

        if (!resource.has_value() || !ctx.world.get().playerTake(player, resource.value())) {
            client->sendError();
        } else {
            client->sendSuccess();
        }
    });
    return true;
}

bool PlayerCommands::move(CommandCtx& ctx, Move move) {
    ctx.client->setTimeout(7, [&ctx, id = ctx.client->playerID(), move]() {
        static std::unordered_map<Move, std::function<void(const game::entity::Player* player)>> map{
            {Move::kForward, [](const game::entity::Player* player) { player->moveForward(); }},
            {Move::kLeft, [](const game::entity::Player* player) { player->turnLeft(); }},
            {Move::kRight, [](const game::entity::Player* player) { player->turnRight(); }},
        };
        const player::PlayerData data(ctx, id);

        if (data.valid()) {
            return;
        }
        map.at(move)(data.player());
        data.client()->sendSuccess();
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

PlayerCommands::PlayerData PlayerCommands::getclientData(CommandCtx& ctx, std::uint64_t id) {
    PlayerData data;

    data.client = ctx.clientRegistry.get().findByID(id);
    if (data.client == nullptr) {
        return data;
    }
    game::IEntity* entity = ctx.world.get().entityDatabase().query(id);
    if (entity == nullptr) {
        data.client->sendError();
        return data;
    }
    data.player = dynamic_cast<game::entity::Player*>(entity);
    if (data.player == nullptr) {
        data.client->sendError();
    }
    return data;
}

}  // namespace zappy::server::command
