/*
** EPITECH PROJECT, 2026
** PlayerCommands
** File description:
** PlayerCommands code
*/

#include "zappy/server/commands/PlayerCommands.hpp"

#include <iostream>
#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
namespace zappy::server::command {

PlayerCommands::PlayerCommands(Timer& timer, client::ClientRegistry& clients) : ACommandGroup(timer, clients) {
    this->_commands = {
        {"Forward", [](auto* client, auto params) {}},     {"Right", [](auto* client, auto params) {}},
        {"Left", [](auto* client, auto params) {}},        {"Look", [](auto* client, auto params) {}},
        {"Inventory", [](auto* client, auto params) {}},   {"Broadcast", [](auto* client, auto params) {}},
        {"Connect_nbr", [](auto* client, auto params) {}}, {"Eject", [](auto* client, auto params) {}},
        {"Take", [](auto* client, auto params) {}},        {"Set", [](auto* client, auto params) {}},
        {"Incantation", [](auto* client, auto params) {}},

    };
}

void PlayerCommands::execute(Client* client, std::string_view msg) {
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
