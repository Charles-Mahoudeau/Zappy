/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** MszHandler
*/

#pragma once

#include <cstddef>
#include <functional>
#include <sstream>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class MszHandler {
  public:
    explicit MszHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) {
        std::size_t width = 0;
        std::size_t height = 0;
        if (!(ss >> width >> height)) {
            throw exception::ParseException{"msz: malformed arguments"};
        }
        _state.get().setMapSize(width, height);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
