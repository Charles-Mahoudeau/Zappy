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

PlayerCommands::PlayerCommands(Timer& timer, client::ClientRegistry& clients) : ACommandGroup(timer, clients) {
    this->_commands = {
        {"Forward", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"Right", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"Left", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"Look", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"Inventory", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"Broadcast", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"Connect_nbr", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"Eject", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"Take", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"Set", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"Incantation", [](auto* client, auto params) { return CommandState::kInvalidParam; }},

    };
}

CommandState PlayerCommands::execute(Client* client, std::string_view msg) {
    const CommandData cmd = this->extractCommand(msg);

    if (auto iter = this->_commands.find(cmd.name); iter != this->_commands.end()) {
        return iter->second(client, cmd);
    }
    return CommandState::kInvalidCommand;
}

}  // namespace zappy::server::command
