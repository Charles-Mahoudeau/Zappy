/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Egg
*/

#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "zappy/server/Timer.hpp"
#include "zappy/server/game/AEntity.hpp"
#include "zappy/server/game/IEventEmitter.hpp"
#include "zappy/server/game/IGrid.hpp"

namespace zappy::server::game::entity {
class Egg : public AEntity {
  public:
    explicit Egg(Timer& timer, IGrid& grid, IEventEmitter& eventEmitter, std::string teamName,
                 std::optional<std::uint64_t> parentPlayerId = std::nullopt);
    ~Egg() override = default;

    Egg(const Egg&) = delete;
    Egg& operator=(const Egg&) = delete;

    Egg(Egg&&) = default;
    Egg& operator=(Egg&&) = default;

    [[nodiscard]] std::optional<std::uint64_t> parentPlayerId() const;

  private:
    std::optional<std::uint64_t> _parentPlayerId;
};
}  // namespace zappy::server::game::entity
