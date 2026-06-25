/*
** EPITECH PROJECT, 2026
** GuiCommands
** File description:
** GuiCommands header
*/

#pragma once

#include <string_view>
#include <unordered_map>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"

namespace zappy::server::command {

class GuiCommands : public ACommandGroup {
  public:
    explicit GuiCommands(CommandCtx context);
    ~GuiCommands() override = default;

    GuiCommands(const GuiCommands&) = delete;
    GuiCommands(GuiCommands&&) = delete;
    GuiCommands& operator=(const GuiCommands&) = delete;
    GuiCommands& operator=(GuiCommands&&) = delete;

    void execute(Client* client, std::string_view msg) override;

  private:
    std::unordered_map<std::string_view, CommandInvoker> _commands;

    static bool ignore(const CommandCtx& ctx);
};

}  // namespace zappy::server::command
