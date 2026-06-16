/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PexHandler
*/

#pragma once

#include <functional>
#include <sstream>
#include <string>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class PexHandler {
  public:
    explicit PexHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::string idToken;
        if (!(ss >> idToken)) {
            throw exception::ParseException{"pex: missing player id"};
        }
        (void)parseId(idToken);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
