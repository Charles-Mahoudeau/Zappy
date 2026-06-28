/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EboHandler
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

class EboHandler {
  public:
    explicit EboHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::string eggToken;
        if (!(ss >> eggToken)) {
            throw exception::ParseException{"ebo: missing egg id"};
        }
        ss >> std::ws;
        if (!ss.eof()) {
            throw exception::ParseException{"ebo: unexpected trailing tokens"};
        }

        auto eggId = parseId(eggToken);
        _state.get().eggEvent(eggId, game::EventType::EggHatch);
        _state.get().removeEgg(eggId);
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
