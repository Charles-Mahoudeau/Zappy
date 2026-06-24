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
};

}  // namespace zappy::server::command
