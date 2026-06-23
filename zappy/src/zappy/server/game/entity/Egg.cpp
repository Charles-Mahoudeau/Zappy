/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Egg
*/

#include "Egg.hpp"

#include <cstdint>

namespace zappy::server::game::entity {
Egg::Egg(const std::uint16_t teamId) : _teamId{teamId} {}

void Egg::update() {
    // Empty
}

std::uint16_t Egg::teamId() const { return _teamId; }
}  // namespace zappy::server::game::entity
