/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PicHandler
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

// pic X Y L #n ... — start of incantation at (X,Y) level L by players #n...
class PicHandler {
  public:
    explicit PicHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) {
        std::uint32_t x = 0;
        std::uint32_t y = 0;
        std::uint32_t level = 0;
        if (!(ss >> x >> y >> level)) {
            throw exception::ParseException{"pic: malformed arguments"};
        }
        std::string idToken;
        while (ss >> idToken) {
            _state.get().setPlayerIncanting(parseId(idToken), true);
        }
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
