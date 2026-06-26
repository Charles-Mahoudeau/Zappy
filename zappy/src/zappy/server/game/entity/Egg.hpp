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

#include "zappy/server/game/AEntity.hpp"
#include "zappy/server/game/IEventEmitter.hpp"
#include "zappy/server/game/IGrid.hpp"

namespace zappy::server::game::entity {
class Egg : public AEntity {
  public:
    explicit Egg(IGrid& grid, IEventEmitter& eventEmitter, std::string teamName,
                 std::optional<std::uint64_t> parentPlayerId = std::nullopt);

    /// @brief Update the egg.
    void update() override;

    [[nodiscard]] std::optional<std::uint64_t> parentPlayerId() const;

  private:
    std::optional<std::uint64_t> _parentPlayerId;
};
}  // namespace zappy::server::game::entity
