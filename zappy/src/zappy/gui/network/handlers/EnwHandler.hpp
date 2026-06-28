/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EnwHandler
*/

#pragma once

#include <cstdint>
#include <functional>
#include <istream>
#include <sstream>
#include <string>

#include "zappy/gui/game/EventHandler.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class EnwHandler {
  public:
    explicit EnwHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::string eggToken;
        std::string playerToken;
        std::uint32_t x = 0;
        std::uint32_t y = 0;
        if (!(ss >> eggToken >> playerToken >> x >> y)) {
            throw exception::ParseException{"enw: malformed arguments"};
        }
        ss >> std::ws;
        if (!ss.eof()) {
            throw exception::ParseException{"enw: unexpected trailing tokens"};
        }
        const auto eggId = parseId(eggToken);
        const auto playerId = parseId(playerToken);
        _state.get().addEgg(eggId, playerId, x, y);
        _state.get().eggEvent(eggId, game::EventType::LayEgg);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
