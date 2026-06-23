/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** AEntity
*/

#include "AEntity.hpp"

#include <cstdint>
#include <string>
#include <utility>

#include "IEventEmitter.hpp"
#include "IGrid.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
AEntity::AEntity(IGrid& grid, IEventEmitter& eventEmitter, std::string teamName)
    : _grid{grid}, _eventEmitter{eventEmitter}, _teamName{std::move(teamName)} {}

AEntity::~AEntity() { _grid.get().remove(_id); }

std::uint64_t AEntity::id() const { return _id; }

void AEntity::setId(const std::uint64_t id) { _id = id; }

math::Vector2u AEntity::position() const { return _grid.get().position(_id).value_or({}); }

void AEntity::setPosition(const math::Vector2u position) { _grid.get().setPosition(_id, position); }

std::string_view AEntity::teamName() const { return _teamName; }

IEventEmitter& AEntity::eventEmitter() const { return _eventEmitter.get(); }
}  // namespace zappy::server::game
