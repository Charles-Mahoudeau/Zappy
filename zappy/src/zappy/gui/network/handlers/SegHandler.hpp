/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** SegHandler
*/

#pragma once

#include <functional>
#include <sstream>
#include <string>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

// seg N — team N wins
class SegHandler {
  public:
    explicit SegHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) {
        std::string team;
        if (!(ss >> team)) {
            throw exception::ParseException{"seg: missing team name"};
        }
        _state.get().setWinner(std::move(team));
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
