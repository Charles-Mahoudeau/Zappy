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

GuiCommands::GuiCommands(Timer& timer, client::ClientRegistry& clients)
    : ACommandGroup(timer, clients),
      _commands({
          {"msz", [](auto* client, auto& params) { return GuiCommands::ignore(client, params); }},
          {"bct", [](auto* client, auto& params) { return GuiCommands::ignore(client, params); }},
          {"mct", [](auto* client, auto& params) { return GuiCommands::ignore(client, params); }},
          {"tna", [](auto* client, auto& params) { return GuiCommands::ignore(client, params); }},
          {"ppo", [](auto* client, auto& params) { return GuiCommands::ignore(client, params); }},
          {"plv", [](auto* client, auto& params) { return GuiCommands::ignore(client, params); }},
          {"pin", [](auto* client, auto& params) { return GuiCommands::ignore(client, params); }},
          {"sgt", [](auto* client, auto& params) { return GuiCommands::ignore(client, params); }},
          {"sst", [](auto* client, auto& params) { return GuiCommands::ignore(client, params); }},

      }) {}

void GuiCommands::execute(Client* client, std::string_view msg) {
    CommandData cmd = this->extractCommand(msg);

    if (auto iter = this->_commands.find(cmd.name); iter != this->_commands.end()) {
        if (!iter->second(client, cmd)) {
            std::ignore = client->sendMessage("sbp\n");
        }
    } else {
        std::ignore = client->sendMessage("suc\n");
    }
}

bool GuiCommands::ignore(const Client* client, const CommandData& params) {
    (void)client;
    (void)params;
    return false;
}

}  // namespace zappy::server::command
