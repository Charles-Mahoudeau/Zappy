/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** HandlerFactory
*/

#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>

#include "zappy/gui/game/GameState.hpp"

namespace zappy::gui::network {

class HandlerFactory {
  public:
    HandlerFactory() = delete;

    [[nodiscard]] static std::unordered_map<std::string, std::function<void(std::istringstream&)>> create(
        game::GameState& state);
};

}  // namespace zappy::gui::network
