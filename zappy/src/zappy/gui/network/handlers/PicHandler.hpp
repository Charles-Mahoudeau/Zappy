/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** PicHandler
*/

#pragma once

#include <cstdint>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class PicHandler {
  public:
    explicit PicHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        if (std::uint32_t x = 0, y = 0, level = 0; !(ss >> x >> y >> level)) {
            throw exception::ParseException{"pic: malformed arguments"};
        }
        std::vector<std::uint32_t> ids;
        std::string idToken;
        while (ss >> idToken) {
            ids.push_back(parseId(idToken));
        }
        for (const auto id : ids) {
            _state.get().setPlayerIncanting(id, true);
        }
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
