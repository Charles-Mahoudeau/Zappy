/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PbcHandler
*/

#pragma once

#include <functional>
#include <istream>
#include <sstream>
#include <string>
#include <utility>

#include "zappy/gui/game/EventHandler.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class PbcHandler {
  public:
    explicit PbcHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::string idToken;
        std::string message;
        if (!(ss >> idToken)) {
            throw exception::ParseException{"pbc: missing player id"};
        }
        std::getline(ss >> std::ws, message);
        if (message.empty()) {
            throw exception::ParseException{"pbc: missing message"};
        }
        _state.get().addBroadcast(std::move(message));

        auto playerId = parseId(idToken);
        auto plr = _state.get().getPlayer(playerId);
        render::Vector3 pos;

        if (plr.has_value()) {
            pos = render::Vector3{static_cast<float>(plr->x), 0.0F, static_cast<float>(plr->y)};
        }
        _state.get().removePlayer(playerId);
        _state.get().broadcastEvent(game::EventType::Death, pos);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
