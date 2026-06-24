/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Grid
*/

#include "Grid.hpp"

#include <cstdint>
#include <optional>
#include <span>

#include "Tile.hpp"
#include "zappy/shared/exception/InvalidState.hpp"
#include "zappy/shared/exception/OutOfRange.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
Grid::Grid(const math::Vector2u size) : _size{size} {
    const std::uint32_t tileCount = _size.x * _size.y;

    if (tileCount == 0) {
        throw exception::OutOfRange{"trying to create a grid with 0 width or height"};
    }
    _tiles.reserve(tileCount);
    for (std::uint32_t i = 0; i < tileCount; ++i) {
        _tiles.emplace_back(math::Vector2u{i % _size.x, i / _size.x});
    }
}

math::Vector2u Grid::size() const { return _size; }

std::optional<math::Vector2u> Grid::position(const std::uint64_t entityId) const {
    const Tile* entityTile = tile(entityId);

    if (entityTile == nullptr) {
        return std::nullopt;
    }
    return entityTile->position();
}

void Grid::setPosition(const std::uint64_t entityId, const math::Vector2u position) {
    if (!isInBounds(position)) {
        throw exception::InvalidArgument{"position is out of bounds"};
    }

    Tile* sourceTile = tile(entityId);

    if (sourceTile == nullptr) {
        throw exception::InvalidState{"entity is not on a tile"};
    }
    if (!sourceTile->removeEntity(entityId)) {
        throw exception::InvalidState{"entity is not on the source tile (this should never happen)"};
    }

    Tile& destinationTile = tile(position);

    destinationTile.addEntity(entityId);
}

void Grid::remove(const std::uint64_t entityId) {
    Tile* sourceTile = tile(entityId);

    if (sourceTile == nullptr) {
        return;
    }
    sourceTile->removeEntity(entityId);
}

std::span<const Tile> Grid::tiles() const { return _tiles; }

std::span<Tile> Grid::tiles() { return _tiles; }

const Tile& Grid::tile(const math::Vector2u position) const {
    if (!isInBounds(position)) {
        throw exception::OutOfRange{"trying to access tile out of bounds"};
    }
    return _tiles.at((position.y * _size.x) + position.x);
}

Tile& Grid::tile(const math::Vector2u position) {
    if (!isInBounds(position)) {
        throw exception::OutOfRange{"trying to access tile out of bounds"};
    }
    return _tiles.at((position.y * _size.x) + position.x);
}

const Tile* Grid::tile(const std::uint64_t entityId) const {
    for (const auto& tile : _tiles) {
        if (tile.hasEntity(entityId)) {
            return &tile;
        }
    }
    return nullptr;
}

Tile* Grid::tile(const std::uint64_t entityId) {
    for (auto& tile : _tiles) {
        if (tile.hasEntity(entityId)) {
            return &tile;
        }
    }
    return nullptr;
}

bool Grid::isInBounds(const math::Vector2u position) const { return position.x < _size.x && position.y < _size.y; }
}  // namespace zappy::server::game
