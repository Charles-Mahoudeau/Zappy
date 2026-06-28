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
#include "zappy/gui/render/utils/Vector3.hpp"
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
        auto plr = _state.get().getPlayer(playerId);
        render::Vector3 pos;

        if (plr.has_value()) {
            pos = render::Vector3{static_cast<float>(plr->x), 0.0F, static_cast<float>(plr->y)};
        }
        _state.get().setPlayerLevel(playerId, level);
        _state.get().broadcastEvent(game::EventType::LevelUp, pos);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
