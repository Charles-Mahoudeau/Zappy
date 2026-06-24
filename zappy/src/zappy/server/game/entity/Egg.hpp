/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Egg
*/

#pragma once

#include "zappy/server/game/AEntity.hpp"

namespace zappy::server::game::entity {
class Egg : public AEntity {
  public:
    using AEntity::AEntity;

    /// @brief Update the egg.
    void update() override;
};
}  // namespace zappy::server::game::entity
