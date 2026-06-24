/*
** EPITECH PROJECT, 2026
** ACommandGroup
** File description:
** ACommandGroup code
*/

#include "zappy/server/commands/ACommandGroup.hpp"

#include <iterator>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/World.hpp"
#include "zappy/shared/io/Logger.hpp"

namespace zappy::server::command {

ACommandGroup::ACommandGroup(Timer& timer, client::ClientRegistry& clients, game::World& world, io::Logger& logger)
    : _ctx{.timer = timer, .clientRegistry = clients, .world = world, .logger = logger} {}

ICommandGroup::CommandCtx& ACommandGroup::commandCtx() {
    this->_ctx.data.name.clear();
    this->_ctx.data.params.clear();
    return this->_ctx;
}

void ACommandGroup::operator()(Client* client, std::string_view cmd) { this->execute(client, cmd); }

ICommandGroup::CommandData ACommandGroup::extractCommand(std::string_view msg) {
    CommandData cmd;
    std::istringstream iss{std::string(msg)};

    iss >> cmd.name;
    cmd.params =
        std::vector<std::string>((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    return cmd;
}

}  // namespace zappy::server::command
