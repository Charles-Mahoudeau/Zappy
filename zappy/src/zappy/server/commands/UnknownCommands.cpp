/*
** EPITECH PROJECT, 2026
** UnknownCommands
** File description:
** UnknownCommands code
*/

#include "zappy/server/commands/UnknownCommands.hpp"

#include <format>
#include <string_view>
#include <tuple>

#include "zappy/server/client/Client.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"
#include "zappy/shared/io/Logger.hpp"

namespace zappy::server::command {

void UnknownCommands::execute(Client* client, [[maybe_unused]] const std::string_view cmd) {
    CommandCtx ctx = this->commandCtx();

    if (!ctx.data.params.empty()) {
        std::ignore = client->sendMessage("ko\n");
    }
    const auto& askedTeam = ctx.data.name;

    if (UnknownCommands::guiType(askedTeam, client, ctx.logger.get())) {
        return;
    }
    if (!UnknownCommands::PlayerType(askedTeam, client, ctx)) {
        std::ignore = client->sendMessage("ko\n");
    }
}

bool UnknownCommands::guiType(std::string_view askedTeam, Client* client, io::Logger& logger) {
    if (askedTeam == "GRAPHIC") {
        logger.info(std::format("Client {} registered as GUI", client->address().string()));
        client->changeType(Client::Type::kGui);
        return true;
    }
    return false;
}

bool UnknownCommands::PlayerType(std::string_view askedTeam, Client* client, CommandCtx& ctx) {
    auto& teams = ctx.teamRegistry.get();
    auto& world = ctx.world.get();
    auto& logger = ctx.logger.get();

    try {
        teams.addToTeam(askedTeam, client->address());
        auto worldSize = world.size();

        auto result = world.hatchRandomEgg(askedTeam);
        if (!result.has_value()) {
            logger.error(std::format("Fail to hatch egg for {}", client->address().string()));
            return false;
        }
        client->setPlayerID(result.value());
        client->changeType(Client::Type::kPlayer);
        std::ignore = client->sendMessage(std::format("{}\n", world.eggCount(askedTeam)));
        std::ignore = client->sendMessage(std::format("{} {}\n", worldSize.x, worldSize.y));
    } catch (const exception::InvalidArgument& err) {
        return false;
    }
    logger.info(std::format("Client {} registered to {}", client->address().string(), askedTeam));
    return true;
}

}  // namespace zappy::server::command
