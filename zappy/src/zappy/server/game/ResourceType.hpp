/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ResourceType
*/

#pragma once

#include <cstdint>

namespace zappy::server::game {
enum class ResourceType : std::uint8_t {
    kFood,
    kLinemate,
    kDeraumere,
    kSibur,
    kMendiane,
    kPhiras,
    kThystame,
    kCount,
};
}  // namespace zappy::server::game
