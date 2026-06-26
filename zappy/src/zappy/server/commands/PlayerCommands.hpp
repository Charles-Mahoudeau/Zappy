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
#include "zappy/server/game/entity/Player.hpp"

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
    struct PlayerData {
        game::entity::Player* player = nullptr;
        Client* client = nullptr;
    };

    std::unordered_map<std::string_view, CommandInvoker> _commands;

    static bool ignore(const CommandCtx& ctx);

    static void inventory(CommandCtx& ctx);
    static void take(CommandCtx& ctx);

    static PlayerData idToPlayerData(CommandCtx& ctx, std::uint64_t id);
};

}  // namespace zappy::server::command
