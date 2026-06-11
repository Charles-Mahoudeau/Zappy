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
#include <tuple>
#include <unordered_map>

#include "zappy/server/game/Inventory.hpp"
#include "zappy/server/game/ResourceType.hpp"

namespace zappy::server::game::entity {
void Player::update() {
    if (_freezeTime > 0) {
        --_freezeTime;
    }
}

std::uint16_t Player::lifetimeLeft() const { return _lifetimeLeft; }

bool Player::alive() const { return _lifetimeLeft > 0; }

void Player::kill() { _lifetimeLeft = 0; }

bool Player::frozen() const { return _freezeTime > 0; }

void Player::freeze(const std::uint32_t time) { _freezeTime = time; }

std::uint8_t Player::level() const { return _level; }

std::expected<std::uint8_t, std::string> Player::levelUp() {
    if (_level >= kMaxLevel) {
        return std::unexpected{"Max level reached"};
    }
    ++_level;
    return _level;
}

Player::Direction Player::direction() const { return _direction; }

Player::Direction Player::turnRight() {
    _direction = std::get<0>(turnMap().at(_direction));
    return _direction;
}

Player::Direction Player::turnLeft() {
    _direction = std::get<1>(turnMap().at(_direction));
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
    return true;
}

const std::unordered_map<Player::Direction, std::tuple<Player::Direction, Player::Direction>>& Player::turnMap() {
    static const std::unordered_map<Direction, std::tuple<Direction, Direction>> turnMap{
        {Direction::kNorth, {Direction::kWest, Direction::kEast}},
        {Direction::kEast, {Direction::kNorth, Direction::kSouth}},
        {Direction::kSouth, {Direction::kEast, Direction::kWest}},
        {Direction::kWest, {Direction::kSouth, Direction::kNorth}},
    };

    return turnMap;
}
}  // namespace zappy::server::game::entity
