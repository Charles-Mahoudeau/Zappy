/*
** EPITECH PROJECT, 2026
** GuiCommands
** File description:
** GuiCommands header
*/

#pragma once

#include <functional>
#include <string_view>
#include <unordered_map>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"

namespace zappy::server::command {

class GuiCommands : public ACommandGroup {
  public:
    GuiCommands(Timer& timer, client::ClientRegistry& clients);
    ~GuiCommands() override = default;

    GuiCommands(const GuiCommands&) = delete;
    GuiCommands(GuiCommands&&) = delete;
    GuiCommands& operator=(const GuiCommands&) = delete;
    GuiCommands& operator=(GuiCommands&&) = delete;

    void execute(Client* client, std::string_view msg) override;

  private:
    using CommandInvoker = std::function<void(Client* client, CommandData)>;

    std::unordered_map<std::string_view, CommandInvoker> _commands;
};

}  // namespace zappy::server::command
