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

#include "zappy/gui/game/EventHandler.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"
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

        auto playerId = parseId(idToken);
        auto plr = _state.get().getPlayer(playerId);
        render::Vector3 pos;

        if (plr.has_value()) {
            pos = render::Vector3{static_cast<float>(plr->x), 0.0F, static_cast<float>(plr->y)};
        }
        // push function
        _state.get().broadcastEvent(game::EventType::LevelUp, pos);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
