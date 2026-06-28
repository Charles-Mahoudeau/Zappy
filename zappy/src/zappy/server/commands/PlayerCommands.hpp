/*
** EPITECH PROJECT, 2026
** PlayerCommands
** File description:
** PlayerCommands header
*/

#pragma once

#include <cstdint>
#include <string_view>
#include <unordered_map>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"

namespace zappy::server::command {

class PlayerCommands : public ACommandGroup {
  public:
    explicit PlayerCommands(CommandCtx context);
    ~PlayerCommands() override = default;

    PlayerCommands(const PlayerCommands&) = delete;
    PlayerCommands(PlayerCommands&&) = delete;
    PlayerCommands& operator=(const PlayerCommands&) = delete;
    PlayerCommands& operator=(PlayerCommands&&) = delete;

    void execute(Client* client, std::string_view msg) override;

  private:
    static constexpr std::uint16_t kIncantationTimeLimit{300};

    static bool ignore(const CommandCtx& ctx);
    static bool inventory(CommandCtx& ctx);
    static bool broadcast(CommandCtx& ctx);
    static bool fork(CommandCtx& ctx);
    static bool eject(CommandCtx& ctx);
    static bool look(CommandCtx& ctx);
    static bool connectNb(CommandCtx& ctx);
    static bool incantation(const CommandCtx& ctx);

    std::unordered_map<std::string_view, CommandInvoker> _commands;
};

}  // namespace zappy::server::command
