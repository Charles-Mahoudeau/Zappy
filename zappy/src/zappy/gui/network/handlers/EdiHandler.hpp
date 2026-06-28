/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EdiHandler
*/

#pragma once

#include <functional>
#include <istream>
#include <sstream>
#include <string>

#include "zappy/gui/game/EventHandler.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class EdiHandler {
  public:
    explicit EdiHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::string eggToken;
        if (!(ss >> eggToken)) {
            throw exception::ParseException{"edi: missing egg id"};
        }
        ss >> std::ws;
        if (!ss.eof()) {
            throw exception::ParseException{"edi: unexpected trailing tokens"};
        }

        auto eggId = parseId(eggToken);
        auto egg = _state.get().getEgg(eggId);
        render::Vector3 pos;

        if (egg.has_value()) {
            pos = render::Vector3{static_cast<float>(egg->x), 0.0F, static_cast<float>(egg->y)};
        }
        _state.get().removeEgg(eggId);
        _state.get().broadcastEvent(game::EventType::EggDeath, pos);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
