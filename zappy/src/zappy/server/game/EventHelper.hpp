/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EventHelper
*/

#pragma once

#include <string>

#include "Event.hpp"

namespace zappy::server::game {
class EventHelper {
  public:
    [[nodiscard]] static std::string toWire(const Event& event);
};
}  // namespace zappy::server::game
