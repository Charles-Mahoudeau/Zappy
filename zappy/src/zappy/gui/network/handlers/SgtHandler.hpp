/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** SgtHandler
*/

#pragma once

#include <cstdint>
#include <functional>
#include <sstream>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class SgtHandler {
  public:
    explicit SgtHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) {
        std::uint32_t t = 0;
        if (!(ss >> t)) {
            throw exception::ParseException{"sgt: missing time unit"};
        }
        _state.get().setTimeUnit(t);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
