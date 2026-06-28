/*
** EPITECH PROJECT, 2026
** UnknownCommands
** File description:
** UnknownCommands code
*/

#include "zappy/server/commands/UnknownCommands.hpp"

#include <format>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/game/WireHelper.hpp"
#include "zappy/server/game/World.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"
#include "zappy/shared/io/Logger.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::command {

void UnknownCommands::execute(Client* client, [[maybe_unused]] const std::string_view cmd) {
    CommandCtx ctx = this->commandCtx();

    if (!ctx.data.params.empty()) {
        client->sendError();
        return;
    }
    const auto& askedTeam = ctx.data.name;

    if (UnknownCommands::handleGuiType(askedTeam, client, ctx)) {
        return;
    }
    if (!UnknownCommands::handlePlayerType(askedTeam, client, ctx)) {
        client->sendError();
    }
}

bool UnknownCommands::handleGuiType(std::string_view askedTeam, Client* client, const CommandCtx& ctx) {
    const auto& logger = ctx.logger.get();

    if (askedTeam != "GRAPHIC") {
        return false;
    }
    client->changeType(Client::Type::kGui);
    logger.info("Client {} registered as GUI", client->address().string());

    const game::World& world = ctx.world.get();
    const math::Vector2u worldSize = world.size();
    std::stringstream gameState;

    gameState << std::format("msz {} {}\n", worldSize.x, worldSize.y);
    gameState << std::format("sgt {}\n", ctx.timer.get().frequency());
    gameState << game::WireHelper::worldToBctCommands(ctx.world.get());
    for (const std::string& teamName : ctx.teamRegistry.get().teamNames()) {
        gameState << std::format("tna {}\n", teamName);
    }
    gameState << game::WireHelper::worldToEnwCommands(ctx.world.get());
    std::ignore = client->sendMessage(gameState.str());
    return true;
}

bool UnknownCommands::handlePlayerType(std::string_view askedTeam, Client* client, CommandCtx& ctx) {
    auto& teams = ctx.teamRegistry.get();
    auto& world = ctx.world.get();
    auto& logger = ctx.logger.get();

    try {
        auto worldSize = world.size();
        auto playerId = world.hatchRandomEgg(askedTeam);

        if (!playerId.has_value()) {
            logger.error("Fail to hatch egg for {}", client->address().string());
            return false;
        }
        teams.addToTeam(askedTeam, client->address());
        client->setPlayerID(playerId.value());
        client->changeType(Client::Type::kPlayer);
        ctx.logger.get().info("Created new player #{}", *playerId);
        std::ignore = client->sendMessage("{}\n", world.eggCount(askedTeam));
        std::ignore = client->sendMessage("{} {}\n", worldSize.x, worldSize.y);

    } catch (const exception::InvalidArgument& err) {
        logger.error("Fail client {} specialization : {}", client->address().string(), err.what());
        return false;
    }
    logger.info("Client {} registered to {}", client->address().string(), askedTeam);
    return true;
}

}  // namespace zappy::server::command
