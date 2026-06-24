/*
** EPITECH PROJECT, 2026
** GuiCommands
** File description:
** GuiCommands code
*/

#include "zappy/server/commands/GuiCommands.hpp"

#include <iostream>
#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
namespace zappy::server::command {

GuiCommands::GuiCommands(Timer& timer, client::ClientRegistry& clients) : ACommandGroup(timer, clients) {
    this->_commands = {
        {"msz", [](auto* client, auto params) {}}, {"bct", [](auto* client, auto params) {}},
        {"mct", [](auto* client, auto params) {}}, {"tna", [](auto* client, auto params) {}},
        {"ppo", [](auto* client, auto params) {}}, {"plv", [](auto* client, auto params) {}},
        {"pin", [](auto* client, auto params) {}}, {"sgt", [](auto* client, auto params) {}},
        {"sst", [](auto* client, auto params) {}},

    };
}

void GuiCommands::execute(Client* client, std::string_view msg) {
    const CommandData cmd = this->extractCommand(msg);

    if (auto iter = this->_commands.find(cmd.name); iter != this->_commands.end()) {
        iter->second(client, cmd);
        return;
    }

    if (!client->sendMessage("suc\n")) {
        std::cerr << "Fail to notice fail command on " << client->address() << "\n";
    }
}

}  // namespace zappy::server::command
