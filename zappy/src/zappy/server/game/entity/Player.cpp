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

#include "zappy/server/game/Event.hpp"
#include "zappy/server/game/IEventEmitter.hpp"
#include "zappy/server/game/Inventory.hpp"
#include "zappy/server/game/ResourceType.hpp"

namespace zappy::server::game::entity {
Player::Player(IEventEmitter& eventEmitter, const std::uint16_t teamId)
    : _eventEmitter{eventEmitter}, _teamId{teamId} {}

void Player::update() {
    if (!alive()) {
        return;
    }
    --_lifetimeLeft;
    if (_lifetimeLeft == 0) {
        _eventEmitter.get().pushEvent(PlayerDeathEvent{
            .playerId = id(),
        });
    }
}

std::uint16_t Player::teamId() const { return _teamId; }

std::uint32_t Player::lifetimeLeft() const { return _lifetimeLeft; }

bool Player::alive() const { return _lifetimeLeft > 0; }

void Player::kill() {
    if (!alive()) {
        return;
    }
    _lifetimeLeft = 0;
    _eventEmitter.get().pushEvent(PlayerDeathEvent{
        .playerId = id(),
    });
}

std::uint8_t Player::level() const { return _level; }

std::expected<std::uint8_t, std::string> Player::levelUp() {
    if (_level >= kMaxLevel) {
        return std::unexpected{"Max level reached"};
    }
    ++_level;
    _eventEmitter.get().pushEvent(PlayerLevelEvent{
        .playerId = id(),
        .level = _level,
    });
    return _level;
}

math::Direction Player::direction() const { return _direction; }

math::Direction Player::turnLeft() {
    _direction = math::direction::turnLeft(_direction);
    // TODO: Send new position with orientation
    return _direction;
}

math::Direction Player::turnRight() {
    _direction = math::direction::turnRight(_direction);
    // TODO: Send new position with orientation
    return _direction;
}

Inventory& Player::inventory() { return _inventory; }

const Inventory& Player::inventory() const { return _inventory; }

bool Player::eat() {
    if (_inventory.resourceCount(ResourceType::kFood) == 0) {
        return false;
    }
    _inventory.removeResource(ResourceType::kFood);
    _lifetimeLeft += kTimeUnitsPerFood;
    _eventEmitter.get().pushEvent(PlayerInventoryEvent{
        .playerId = id(),
        .inventory = _inventory,
    });
    return true;
}
}  // namespace zappy::server::game::entity
