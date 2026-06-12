/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** BctHandler
*/

#pragma once

#include <cstddef>
#include <functional>
#include <sstream>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class BctHandler {
  public:
    explicit BctHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::size_t x = 0;
        std::size_t y = 0;
        if (!(ss >> x >> y)) {
            throw exception::ParseException{"bct: malformed coordinates"};
        }
        _state.get().setTile(x, y, parseResources(ss));
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
