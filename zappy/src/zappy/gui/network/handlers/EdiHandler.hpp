/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EdiHandler
*/

#pragma once

#include <functional>
#include <sstream>
#include <string>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class EdiHandler {
  public:
    explicit EdiHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) {
        std::string eggToken;
        if (!(ss >> eggToken)) {
            throw exception::ParseException{"edi: missing egg id"};
        }
        _state.get().removeEgg(parseId(eggToken));
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
