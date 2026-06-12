/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TnaHandler
*/

#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <utility>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class TnaHandler {
  public:
    explicit TnaHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) {
        std::string name;
        if (!(ss >> name)) {
            throw exception::ParseException{"tna: missing team name"};
        }
        _state.get().addTeam(std::move(name));
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
