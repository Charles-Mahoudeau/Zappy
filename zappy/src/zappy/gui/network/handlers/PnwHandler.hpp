/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PnwHandler
*/

#pragma once

#include <cstdint>
#include <functional>
#include <sstream>
#include <string>
#include <utility>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class PnwHandler {
  public:
    explicit PnwHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::string idToken;
        std::uint32_t x = 0;
        std::uint32_t y = 0;
        std::uint32_t o = 0;
        std::uint32_t level = 0;
        std::string team;
        if (!(ss >> idToken >> x >> y >> o >> level >> team)) {
            throw exception::ParseException{"pnw: malformed arguments"};
        }
        _state.get().addPlayer(parseId(idToken), x, y, parseOrientation(o), level, std::move(team));
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
