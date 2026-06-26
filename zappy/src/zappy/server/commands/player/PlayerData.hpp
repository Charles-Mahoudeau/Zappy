/*
** EPITECH PROJECT, 2026
** PlayerData
** File description:
** PlayerData header
*/

#pragma once

#include <cstdint>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/entity/Player.hpp"

namespace zappy::server::command::player {

class PlayerData {
  public:
    explicit PlayerData(ICommandGroup::CommandCtx& ctx, std::uint64_t id);
    ~PlayerData() = default;

    PlayerData(const PlayerData&) = delete;
    PlayerData(PlayerData&&) = delete;
    PlayerData& operator=(const PlayerData&) = delete;
    PlayerData& operator=(PlayerData&&) = delete;

    [[nodiscard]] bool valid() const;

    game::entity::Player* player();
    [[nodiscard]] const game::entity::Player* player() const;

    [[nodiscard]] const Client* client() const;
    Client* client();

  private:
    game::entity::Player* _player = nullptr;
    Client* _client = nullptr;
    // Members
};

}  // namespace zappy::server::command::player
