/*
** EPITECH PROJECT, 2026
** MoveCommand
** File description:
** MoveCommand code
*/

#include "zappy/server/commands/player/MoveCommand.hpp"

#include <functional>
#include <unordered_map>

#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/commands/player/PlayerData.hpp"
#include "zappy/server/game/entity/Player.hpp"
namespace zappy::server::command::player {

bool MoveCommand::forward(ICommandGroup::CommandCtx& ctx) { return MoveCommand::move(ctx, Move::kForward); }
bool MoveCommand::right(ICommandGroup::CommandCtx& ctx) { return MoveCommand::move(ctx, Move::kRight); }
bool MoveCommand::left(ICommandGroup::CommandCtx& ctx) { return MoveCommand::move(ctx, Move::kLeft); }

bool MoveCommand::move(ICommandGroup::CommandCtx& ctx, Move move) {
    ctx.client->setTimeout(7, [&ctx, id = ctx.client->playerID(), move]() {
        static std::unordered_map<Move, std::function<void(game::entity::Player * player)>> map{
            {Move::kForward, [](game::entity::Player* player) { player->moveForward(); }},
            {Move::kLeft, [](game::entity::Player* player) { player->turnLeft(); }},
            {Move::kRight, [](game::entity::Player* player) { player->turnRight(); }},
        };
        PlayerData data(ctx, id);

        if (!data.valid()) {
            return;
        }
        map.at(move)(data.player());
        data.client()->sendSuccess();
    });
    return true;
}

}  // namespace zappy::server::command::player
