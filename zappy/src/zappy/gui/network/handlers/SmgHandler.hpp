/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** SmgHandler
*/

#pragma once

#include <functional>
#include <istream>
#include <sstream>
#include <string>
#include <utility>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class SmgHandler {
  public:
    explicit SmgHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::string message;
        std::getline(ss >> std::ws, message);
        if (message.empty()) {
            throw exception::ParseException{"smg: missing message"};
        }
        _state.get().addBroadcast(std::move(message));
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
