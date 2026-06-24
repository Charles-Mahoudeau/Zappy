/*
** EPITECH PROJECT, 2026
** GuiCommands
** File description:
** GuiCommands header
*/

#pragma once

#include <string_view>
#include <unordered_map>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/World.hpp"
#include "zappy/shared/io/Logger.hpp"

namespace zappy::server::command {

class GuiCommands : public ACommandGroup {
  public:
    GuiCommands(Timer& timer, client::ClientRegistry& clients, game::World& world, io::Logger& logger);
    ~GuiCommands() override = default;

    GuiCommands(const GuiCommands&) = delete;
    GuiCommands(GuiCommands&&) = delete;
    GuiCommands& operator=(const GuiCommands&) = delete;
    GuiCommands& operator=(GuiCommands&&) = delete;

    void execute(Client* client, std::string_view msg) override;

  private:
    std::unordered_map<std::string_view, CommandInvoker> _commands;

    static bool ignore(CommandCtx& ctx);
};

}  // namespace zappy::server::command
