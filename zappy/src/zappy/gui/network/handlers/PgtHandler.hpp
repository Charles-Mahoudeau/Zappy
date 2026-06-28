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

    void operator()(std::istringstream& ss) const {
        std::string idToken;
        if (std::uint32_t resource = 0; !(ss >> idToken >> resource)) {
            throw exception::ParseException{"pgt: malformed arguments"};
        }
        auto player = parseId(idToken);
        _state.get().playerEvent(player, game::EventType::Eat);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
