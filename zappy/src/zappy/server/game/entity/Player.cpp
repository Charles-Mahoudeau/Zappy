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
Player::Player(Timer& timer, IGrid& grid, IEventEmitter& eventEmitter, std::string teamName)
    : AEntity{timer, grid, eventEmitter, std::move(teamName)} {
    _foodTimerId = timer.scheduleEvery(kTimeUnitsPerFood, [this] {
        if (_foodTicksLeft > 0) {
            --_foodTicksLeft;
            return;
        }
        if (!eat()) {
            kill();
        }
    });
}

Player::~Player() {
    if (_foodTimerId.has_value()) {
        timer().unschedule(_foodTimerId.value());
    }
}

bool Player::alive() const { return _alive; }

void Player::kill() {
    if (!_alive) {
        return;
    }
    _alive = false;
    if (_foodTimerId.has_value()) {
        timer().unschedule(_foodTimerId.value());
        _foodTimerId = std::nullopt;
    }
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

const Inventory& Player::inventory() const { return _inventory; }

bool Player::eat() {
    if (!_alive) {
        return false;
    }
    if (_inventory.resourceCount(ResourceType::kFood) == 0) {
        return false;
    }
    _inventory.removeResource(ResourceType::kFood);
    _foodTicksLeft += kTimeUnitsPerFood;
    eventEmitter().pushEvent(PlayerInventoryEvent{
        .playerId = id(),
        .position = position(),
        .inventory = _inventory,
    });
    return true;
}
}  // namespace zappy::server::game::entity
