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
#include <utility>
#include <vector>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server::command {
ACommandGroup::ACommandGroup(CommandCtx context) : _ctx{std::move(context)} {}

void ACommandGroup::operator()(Client* client, const std::string_view cmd) {
    _ctx.client = client;
    _ctx.data = extractCommand(cmd);
    execute(client, cmd);
}

ICommandGroup::CommandCtx& ACommandGroup::commandCtx() { return _ctx; }

ICommandGroup::CommandData ACommandGroup::extractCommand(const std::string_view msg) {
    CommandData cmd;
    std::istringstream iss{std::string(msg)};

    iss >> cmd.name;
    cmd.params = std::vector(std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{});
    return cmd;
}

void ACommandGroup::trySendToAddress(CommandCtx& ctx, network::Address addr, std::string_view msg) {
    const Client* client = ctx.clientRegistry.get().findByAddress(addr);
    if (client == nullptr) {
        return;
    }
    std::ignore = client->sendMessage(msg);
}

}  // namespace zappy::server::command
