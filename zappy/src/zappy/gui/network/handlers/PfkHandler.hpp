/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PfkHandler
*/

#pragma once

#include <functional>
#include <sstream>
#include <string>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class PfkHandler {
  public:
    explicit PfkHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::string idToken;
        if (!(ss >> idToken)) {
            throw exception::ParseException{"pfk: missing player id"};
        }
        (void)parseId(idToken);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
