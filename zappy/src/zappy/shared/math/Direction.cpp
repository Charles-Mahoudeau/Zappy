/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Direction
*/

#include "Direction.hpp"

#include <cstdint>
#include <random>
#include <tuple>
#include <unordered_map>

namespace zappy::math::direction {
namespace {
[[nodiscard]] const std::unordered_map<Direction, std::tuple<Direction, Direction>>& turnMap() {
    using enum Direction;

    static const std::unordered_map<Direction, std::tuple<Direction, Direction>> turnMap{
        {kNorth, {kWest, kEast}},
        {kEast, {kNorth, kSouth}},
        {kSouth, {kEast, kWest}},
        {kWest, {kSouth, kNorth}},
    };

    return turnMap;
}
}  // namespace

Direction turnLeft(const Direction direction) { return std::get<0>(turnMap().at(direction)); }

Direction turnRight(const Direction direction) { return std::get<1>(turnMap().at(direction)); }

Direction random() {
    static std::random_device randomDevice;
    static std::mt19937 randomEngine{randomDevice()};
    std::uniform_int_distribution<std::uint8_t> distribution{0, 3};

    return Direction{distribution(randomEngine)};
}
}  // namespace zappy::math::direction
