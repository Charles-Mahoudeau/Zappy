/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Player
*/

#include "Player.hpp"

#include <cstdint>
#include <expected>
#include <string>

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
        .orientation = direction(),
    });
}

void Player::moveForward() {
    const math::Vector2u gridSize = this->gridSize();
    math::Vector2u pos = this->position();

    switch (this->_direction) {
        case math::Direction::kNorth:
            pos.y = (pos.y == gridSize.y) ? 0 : pos.y + 1;
            break;
        case math::Direction::kEast:
            pos.x = (pos.x == gridSize.x) ? 0 : pos.x + 1;
            break;
        case math::Direction::kSouth:
            pos.y = (pos.y == 0) ? gridSize.y : pos.y - 1;
            break;
        case math::Direction::kWest:
            pos.x = (pos.x == 0) ? gridSize.x : pos.x - 1;
            break;
    }
    this->setPosition(pos);
}

math::Direction Player::direction() const { return _direction; }

math::Direction Player::turnLeft() {
    _direction = math::direction::turnLeft(_direction);
    eventEmitter().pushEvent(PlayerPositionEvent{
        .playerId = id(),
        .position = position(),
        .orientation = _direction,
    });
    return _direction;
}

math::Direction Player::turnRight() {
    _direction = math::direction::turnRight(_direction);
    eventEmitter().pushEvent(PlayerPositionEvent{
        .playerId = id(),
        .position = position(),
        .orientation = _direction,
    });
    return _direction;
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
