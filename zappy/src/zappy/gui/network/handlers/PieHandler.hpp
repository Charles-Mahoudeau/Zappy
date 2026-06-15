/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PieHandler
*/

#pragma once

#include <cstdint>
#include <functional>
#include <sstream>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class PieHandler {
  public:
    explicit PieHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::uint32_t x = 0;
        std::uint32_t y = 0;
        if (std::uint32_t result = 0; !(ss >> x >> y >> result)) {
            throw exception::ParseException{"pie: malformed arguments"};
        }
        for (const auto& [id, player] : _state.get().players()) {
            if (player.x == x && player.y == y && player.isIncanting) {
                _state.get().setPlayerIncanting(id, false);
            }
        }
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
