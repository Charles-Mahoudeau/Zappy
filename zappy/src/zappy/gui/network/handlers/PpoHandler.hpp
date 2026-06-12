/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PpoHandler
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

class PpoHandler {
  public:
    explicit PpoHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) {
        std::string idToken;
        std::uint32_t x = 0;
        std::uint32_t y = 0;
        std::uint32_t o = 0;
        if (!(ss >> idToken >> x >> y >> o)) {
            throw exception::ParseException{"ppo: malformed arguments"};
        }
        _state.get().setPlayerPosition(parseId(idToken), x, y, parseOrientation(o));
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
