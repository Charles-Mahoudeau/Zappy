/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Player
*/

#include "Player.hpp"

#include <cstdint>
#include <expected>
#include <functional>
#include <string>
#include <unordered_map>

#include "zappy/server/game/AEntity.hpp"
#include "zappy/server/game/Event.hpp"
#include "zappy/server/game/IEventEmitter.hpp"
#include "zappy/server/game/Inventory.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/shared/math/Direction.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game::entity {
void Player::update() {
    if (!alive()) {
        return;
    }
    --_lifetimeLeft;
    if (_lifetimeLeft == 0) {
        eventEmitter().pushEvent(PlayerDeathEvent{
            .playerId = id(),
        });
    }
}

std::uint32_t Player::lifetimeLeft() const { return _lifetimeLeft; }

bool Player::alive() const { return _lifetimeLeft > 0; }

void Player::kill() {
    if (!alive()) {
        return;
    }
    _lifetimeLeft = 0;
    eventEmitter().pushEvent(PlayerDeathEvent{
        .playerId = id(),
    });
}

std::uint8_t Player::level() const { return _level; }

std::expected<std::uint8_t, std::string> Player::levelUp() {
    if (_level >= kMaxLevel) {
        return std::unexpected{"Max level reached"};
    }
    ++_level;
    eventEmitter().pushEvent(PlayerLevelEvent{
        .playerId = id(),
        .level = _level,
    });
    return _level;
}

void Player::setPosition(const math::Vector2u position) {
    AEntity::setPosition(position);
    eventEmitter().pushEvent(PlayerPositionEvent{
        .playerId = id(),
        .position = position,
        .orientation = orientation(),
    });
}

void Player::moveForward() {
    using enum math::Direction;
    static std::unordered_map<math::Direction, std::function<void(const math::Vector2u, math::Vector2u&)>> map{
        {kNorth, [](math::Vector2u grid, math::Vector2u& pos) { pos.y = (pos.y + 1) % grid.y; }},
        {kEast, [](math::Vector2u grid, math::Vector2u& pos) { pos.x = (pos.x + 1) % grid.x; }},
        {kSouth, [](math::Vector2u grid, math::Vector2u& pos) { pos.y = (pos.y + grid.y - 1) % grid.y; }},
        {kWest, [](math::Vector2u grid, math::Vector2u& pos) { pos.x = (pos.x + grid.x - 1) % grid.x; }},
    };

    math::Vector2u pos = this->position();

    map.find(this->_orientation)->second(this->gridSize(), pos);

    this->setPosition(pos);
}

math::Direction Player::orientation() const { return _orientation; }

math::Direction Player::turnLeft() {
    _orientation = math::direction::turnLeft(_orientation);
    eventEmitter().pushEvent(PlayerPositionEvent{
        .playerId = id(),
        .position = position(),
        .orientation = _orientation,
    });
    return _orientation;
}

math::Direction Player::turnRight() {
    _orientation = math::direction::turnRight(_orientation);
    eventEmitter().pushEvent(PlayerPositionEvent{
        .playerId = id(),
        .position = position(),
        .orientation = _orientation,
    });
    return _orientation;
}

bool Player::eat() {
    if (_inventory.resourceCount(ResourceType::kFood) == 0) {
        return false;
    }
    _inventory.removeResource(ResourceType::kFood);
    _lifetimeLeft += kTimeUnitsPerFood;
    eventEmitter().pushEvent(PlayerInventoryEvent{
        .playerId = id(),
        .position = position(),
        .inventory = _inventory,
    });
    return true;
}

const Inventory& Player::inventory() const { return this->_inventory; }

void Player::take(ResourceType resource) { this->_inventory.addResource(resource); }

bool Player::drop(ResourceType resource) {
    if (this->_inventory.resourceCount(resource) == 0) {
        return false;
    }
    this->_inventory.removeResource(resource);
    return true;
};
}  // namespace zappy::server::game::entity
