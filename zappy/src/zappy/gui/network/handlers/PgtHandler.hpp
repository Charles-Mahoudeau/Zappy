/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PgtHandler
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

class PgtHandler {
  public:
    explicit PgtHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) {
        std::string idToken;
        std::uint32_t resource = 0;
        if (!(ss >> idToken >> resource)) {
            throw exception::ParseException{"pgt: malformed arguments"};
        }
        (void)parseId(idToken);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
