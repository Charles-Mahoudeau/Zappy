/*
** EPITECH PROJECT, 2026
** PlayerCommands
** File description:
** PlayerCommands code
*/

#include "zappy/server/commands/PlayerCommands.hpp"

#include <iostream>
#include <string_view>
#include <tuple>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
namespace zappy::server::command {

PlayerCommands::PlayerCommands(Timer& timer, client::ClientRegistry& clients) : ACommandGroup(timer, clients) {
    this->_commands = {
        {"Forward", [](auto* client, auto params) { return false; }},
        {"Right", [](auto* client, auto params) { return false; }},
        {"Left", [](auto* client, auto params) { return false; }},
        {"Look", [](auto* client, auto params) { return false; }},
        {"Inventory", [](auto* client, auto params) { return false; }},
        {"Broadcast", [](auto* client, auto params) { return false; }},
        {"Connect_nbr", [](auto* client, auto params) { return false; }},
        {"Eject", [](auto* client, auto params) { return false; }},
        {"Take", [](auto* client, auto params) { return false; }},
        {"Set", [](auto* client, auto params) { return false; }},
        {"Incantation", [](auto* client, auto params) { return false; }},

    };
}

void PlayerCommands::execute(Client* client, std::string_view msg) {
    const CommandData cmd = this->extractCommand(msg);

    if (auto iter = this->_commands.find(cmd.name); iter != this->_commands.end()) {
        if (!iter->second(client, cmd)) {
            std::ignore = client->sendMessage("ko\n");
        }
    } else {
        std::ignore = client->sendMessage("ko\n");
    }
}

}  // namespace zappy::server::command
