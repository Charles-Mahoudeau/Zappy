/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PdrHandler
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

class PdrHandler {
  public:
    explicit PdrHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::string idToken;
        std::uint32_t resource = 0;
        if (!(ss >> idToken >> resource)) {
            throw exception::ParseException{"pdr: malformed arguments"};
        }
        (void)parseId(idToken);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
