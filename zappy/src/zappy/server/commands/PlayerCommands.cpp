/*
** EPITECH PROJECT, 2026
** PlayerCommands
** File description:
** PlayerCommands code
*/

#include "zappy/server/commands/PlayerCommands.hpp"

#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"

namespace zappy::server::command {

PlayerCommands::PlayerCommands(Timer& timer, client::ClientRegistry& clients)
    : ACommandGroup(timer, clients),
      _commands({
          {"Forward", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
          {"Right", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
          {"Left", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
          {"Look", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
          {"Inventory", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
          {"Broadcast", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
          {"Connect_nbr", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
          {"Eject", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
          {"Take", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
          {"Set", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
          {"Incantation", [](auto* client, auto& params) { return PlayerCommands::ignore(client, params); }},
      }) {}

void PlayerCommands::execute(Client* client, std::string_view msg) {
    CommandData cmd = this->extractCommand(msg);

    if (auto iter = this->_commands.find(cmd.name); iter != this->_commands.end()) {
        if (!iter->second(client, cmd)) {
            (void)client->sendMessage("ko\n");
        }
    } else {
        (void)client->sendMessage("ko\n");
    }
}

bool PlayerCommands::ignore(Client* client, CommandData& params) {
    (void)client;
    (void)params;
    return false;
}

}  // namespace zappy::server::command
