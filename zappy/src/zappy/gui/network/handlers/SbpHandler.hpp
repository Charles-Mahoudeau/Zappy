/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** SbpHandler
*/

#pragma once

#include <functional>
#include <sstream>

#include "zappy/gui/game/GameState.hpp"

namespace zappy::gui::network::handlers {

class SbpHandler {
  public:
    explicit SbpHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& /*ss*/) {
        // No state change; the GUI can log this if needed
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
