/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** AEntity
*/

#include "AEntity.hpp"

#include <cstdint>

namespace zappy::server::game {
std::uint64_t AEntity::id() const { return _id; }

void AEntity::setId(const std::uint64_t id) { _id = id; }
}  // namespace zappy::server::game
