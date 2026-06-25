/*
** EPITECH PROJECT, 2026
** UnknownCommands
** File description:
** UnknownCommands header
*/

#pragma once

#include <string_view>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/shared/io/Logger.hpp"

namespace zappy::server::command {

class UnknownCommands : public ACommandGroup {
  public:
    using ACommandGroup::ACommandGroup;
    ~UnknownCommands() override = default;

    UnknownCommands(const UnknownCommands&) = delete;
    UnknownCommands(UnknownCommands&&) = delete;
    UnknownCommands& operator=(const UnknownCommands&) = delete;
    UnknownCommands& operator=(UnknownCommands&&) = delete;

    void execute(Client* client, std::string_view cmd) override;

  private:
    static bool guiType(std::string_view askedTeam, Client* client, io::Logger& logger);
    static bool PlayerType(std::string_view askedTeam, Client* client, CommandCtx& ctx);
};

}  // namespace zappy::server::command
