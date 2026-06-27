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
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

#include "zappy/server/Timer.hpp"
#include "zappy/server/game/AEntity.hpp"
#include "zappy/server/game/Event.hpp"
#include "zappy/server/game/IEventEmitter.hpp"
#include "zappy/server/game/IGrid.hpp"
#include "zappy/server/game/Inventory.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/shared/math/Direction.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game::entity {
Player::Player(Timer& timer, IGrid& grid, IEventEmitter& eventEmitter, std::string teamName)
    : AEntity{timer, grid, eventEmitter, std::move(teamName)} {
    _inventory.addResource(ResourceType::kFood, kInitialFoodAmount);
    _foodTimerId = timer.scheduleEvery(kTimeUnitsPerFood, [this] {
        if (!eat()) {
            kill();
        }
    });
}

Player::~Player() {
    if (_foodTimerId.has_value()) {
        try {
            timer().unschedule(_foodTimerId.value());
        } catch (const std::bad_alloc&) {
            std::cerr << "You tried to destroy a Player (maybe to save some memory), but you did not have enough "
                         "memory to do so. That's quite amusing, isn't it?"
                      << std::endl;
        }
    }
}

Player::Player(Player&& other) noexcept
    : AEntity{other},
      _alive{other._alive},
      _level{other._level},
      _orientation{other._orientation},
      _inventory{std::move(other._inventory)},
      _foodTimerId{other._foodTimerId} {
    other._alive = false;
    other._foodTimerId = std::nullopt;
}

Player& Player::operator=(Player&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    AEntity::operator=(std::move(other));
    _alive = other._alive;
    _level = other._level;
    _orientation = other._orientation;
    _inventory = std::move(other._inventory);
    _foodTimerId = other._foodTimerId;
    other._alive = false;
    other._foodTimerId = std::nullopt;
    return *this;
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

const Inventory& Player::inventory() const { return _inventory; }

void Player::take(const ResourceType resource) { _inventory.addResource(resource); }

bool Player::drop(const ResourceType resource) {
    if (_inventory.resourceCount(resource) == 0) {
        return false;
    }
    _inventory.removeResource(resource);
    return true;
}

bool Player::eat() {
    if (!_alive) {
        return false;
    }
    if (_inventory.resourceCount(ResourceType::kFood) == 0) {
        return false;
    }
    _inventory.removeResource(ResourceType::kFood);
    eventEmitter().pushEvent(PlayerInventoryEvent{
        .playerId = id(),
        .position = position(),
        .inventory = _inventory,
    });
    return true;
};
}  // namespace zappy::server::game::entity
