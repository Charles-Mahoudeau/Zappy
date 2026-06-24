/*
** EPITECH PROJECT, 2026
** GuiCommands
** File description:
** GuiCommands code
*/

#include "zappy/server/commands/GuiCommands.hpp"

#include <string_view>
#include <tuple>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
namespace zappy::server::command {

GuiCommands::GuiCommands(Timer& timer, client::ClientRegistry& clients) : ACommandGroup(timer, clients) {
    this->_commands = {
        {"msz", [](auto* client, auto params) {return false; }},
        {"bct", [](auto* client, auto params) {return false; }},
        {"mct", [](auto* client, auto params) {return false; }},
        {"tna", [](auto* client, auto params) {return false; }},
        {"ppo", [](auto* client, auto params) {return false; }},
        {"plv", [](auto* client, auto params) {return false; }},
        {"pin", [](auto* client, auto params) {return false; }},
        {"sgt", [](auto* client, auto params) {return false; }},
        {"sst", [](auto* client, auto params) {return false; }},

    };
}

void GuiCommands::execute(Client* client, std::string_view msg) {
    const CommandData cmd = this->extractCommand(msg);

    if (auto iter = this->_commands.find(cmd.name); iter != this->_commands.end()) {
        if (!iter->second(client, cmd)) {
            std::ignore = client->sendMessage("sbp\n");
        }
    } else {
        std::ignore = client->sendMessage("suc\n");
    }
}

}  // namespace zappy::server::command
