/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** AEntity
*/

#include "AEntity.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "IEventEmitter.hpp"
#include "IGrid.hpp"
#include "zappy/shared/exception/InvalidState.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
AEntity::AEntity(IGrid& grid, IEventEmitter& eventEmitter, std::string teamName)
    : _grid{grid}, _eventEmitter{eventEmitter}, _teamName{std::move(teamName)} {}

std::uint64_t AEntity::id() const { return _id; }

void AEntity::setId(const std::uint64_t id) { _id = id; }

math::Vector2u AEntity::position() const {
    if (const std::optional<math::Vector2u> position = _grid.get().position(_id)) {
        return position.value();
    }
    throw exception::InvalidState{"entity is not on a tile"};
}

void AEntity::setPosition(const math::Vector2u position) { _grid.get().setPosition(_id, position); }

std::string_view AEntity::teamName() const { return _teamName; }

IEventEmitter& AEntity::eventEmitter() const { return _eventEmitter.get(); }

math::Vector2u AEntity::gridSize() const { return this->_grid.get().size(); }

}  // namespace zappy::server::game
