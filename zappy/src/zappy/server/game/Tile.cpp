/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Tile
*/

#include "Tile.hpp"

#include <algorithm>
#include <cstdint>
#include <span>

#include "EntityDatabase.hpp"
#include "World.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
Tile::Tile(const math::Vector2u position) : _position{position} {}

math::Vector2u Tile::position() const { return _position; }

std::uint64_t Tile::count() const { return _entities.size(); }

std::span<const std::uint64_t> Tile::entities() const { return _entities; }

bool Tile::hasEntity(const std::uint64_t entityId) const {
    return std::ranges::find(_entities, entityId) != _entities.end();
}

void Tile::addEntity(const std::uint64_t entityId) { _entities.push_back(entityId); }

bool Tile::removeEntity(const std::uint64_t entityId) {
    if (const auto it = std::ranges::find(_entities, entityId); it != _entities.end()) {
        _entities.erase(it);
        return true;
    }
    return false;
}
}  // namespace zappy::server::game
