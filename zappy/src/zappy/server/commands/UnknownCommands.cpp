/*
** EPITECH PROJECT, 2026
** UnknownCommands
** File description:
** UnknownCommands code
*/

#include "zappy/server/commands/UnknownCommands.hpp"

#include <iostream>
#include <string_view>

#include "zappy/server/client/Client.hpp"

namespace zappy::server::command {

void UnknownCommands::execute(Client* client, std::string_view cmd) {
    std::cout << client->address() << " -> " << cmd << "\n";
}

}  // namespace zappy::server::command
