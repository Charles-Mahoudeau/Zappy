/*
** EPITECH PROJECT, 2026
** GuiCommands
** File description:
** GuiCommands code
*/

#include "zappy/server/commands/GuiCommands.hpp"

#include <string_view>
#include <tuple>
#include <utility>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"

namespace zappy::server::command {

GuiCommands::GuiCommands(CommandCtx context)
    : ACommandGroup{std::move(context)},
      _commands({
          {"msz", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"bct", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"mct", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"tna", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"ppo", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"plv", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"pin", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"sgt", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"sst", [](auto& ctx) { return GuiCommands::ignore(ctx); }},

      }) {}

void GuiCommands::execute(Client* client, [[maybe_unused]] const std::string_view msg) {
    CommandCtx& ctx = this->commandCtx();

    if (const auto iter = this->_commands.find(ctx.data.name); iter != this->_commands.end()) {
        if (!iter->second(ctx)) {
            std::ignore = client->sendMessage("sbp\n");
        }
    } else {
        std::ignore = client->sendMessage("suc\n");
    }
}

bool GuiCommands::ignore(const CommandCtx& ctx) {
    (void)ctx;
    return false;
}

}  // namespace zappy::server::command
