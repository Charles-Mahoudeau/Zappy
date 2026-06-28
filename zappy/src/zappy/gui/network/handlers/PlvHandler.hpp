/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PlvHandler
*/

#pragma once

#include <cstdint>
#include <functional>
#include <sstream>
#include <string>

#include "zappy/gui/game/EventHandler.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class PlvHandler {
  public:
    explicit PlvHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::string idToken;
        std::uint32_t level = 0;
        if (!(ss >> idToken >> level)) {
            throw exception::ParseException{"plv: malformed arguments"};
        }

        auto playerId = parseId(idToken);
        _state.get().playerEvent(playerId, game::EventType::LevelUp);
        _state.get().setPlayerLevel(playerId, level);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
