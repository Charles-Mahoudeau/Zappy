/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PinHandler
*/

#pragma once

#include <cstdint>
#include <functional>
#include <sstream>
#include <string>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class PinHandler {
  public:
    explicit PinHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) {
        std::string idToken;
        std::uint32_t x = 0;
        std::uint32_t y = 0;
        if (!(ss >> idToken >> x >> y)) {
            throw exception::ParseException{"pin: malformed arguments"};
        }
        _state.get().setPlayerInventory(parseId(idToken), x, y, parseResources(ss));
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
