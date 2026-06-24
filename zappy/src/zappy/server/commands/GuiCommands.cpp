/*
** EPITECH PROJECT, 2026
** GuiCommands
** File description:
** GuiCommands code
*/

#include "zappy/server/commands/GuiCommands.hpp"

#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
namespace zappy::server::command {

GuiCommands::GuiCommands(Timer& timer, client::ClientRegistry& clients) : ACommandGroup(timer, clients) {
    this->_commands = {
        {"msz", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"bct", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"mct", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"tna", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"ppo", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"plv", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"pin", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"sgt", [](auto* client, auto params) { return CommandState::kInvalidParam; }},
        {"sst", [](auto* client, auto params) { return CommandState::kInvalidParam; }},

    };
}

CommandState GuiCommands::execute(Client* client, std::string_view msg) {
    const CommandData cmd = this->extractCommand(msg);

    if (auto iter = this->_commands.find(cmd.name); iter != this->_commands.end()) {
        return iter->second(client, cmd);
    }
    return CommandState::kInvalidCommand;
}

}  // namespace zappy::server::command
