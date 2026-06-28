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
#include "zappy/gui/render/utils/Vector3.hpp"
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
        _state.get().eggEvent(eggId, game::EventType::EggDeath);
        _state.get().removeEgg(eggId);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
