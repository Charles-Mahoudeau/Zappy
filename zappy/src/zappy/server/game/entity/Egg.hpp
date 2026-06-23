/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Egg
*/

#pragma once

#include <cstdint>

#include "zappy/server/game/IEntity.hpp"

namespace zappy::server::game::entity {
class Egg : public IEntity {
  public:
    explicit Egg(std::uint16_t teamId);
    ~Egg() override = default;

    Egg(const Egg&) = default;
    Egg& operator=(const Egg&) = default;

    Egg(Egg&&) = default;
    Egg& operator=(Egg&&) = default;

    /// @brief Update the egg.
    void update() override;

    /// @brief Get the team id of the egg.
    /// @return The team id of the egg.
    [[nodiscard]] std::uint16_t teamId() const;

  private:
    std::uint16_t _teamId;
};
}  // namespace zappy::server::game::entity
