/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** HandlerFactory
*/

#pragma once

#include <functional>
#include <map>
#include <sstream>
#include <string>

#include "zappy/gui/game/GameState.hpp"

namespace zappy::gui::network {

using HandlerMap = std::map<std::string, std::function<void(std::istringstream&)>, std::less<>>;

class HandlerFactory {
  public:
    HandlerFactory() = delete;

    [[nodiscard]] static HandlerMap create(game::GameState& state);
};

}  // namespace zappy::gui::network
