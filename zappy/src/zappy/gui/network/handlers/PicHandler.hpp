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

#include "zappy/gui/game/EventHandler.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/handlers/ParseHelpers.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

class PicHandler {
  public:
    explicit PicHandler(game::GameState& state) : _state(state) {}

    void operator()(std::istringstream& ss) const {
        std::uint32_t x = 0;
        std::uint32_t y = 0;
        std::uint32_t level = 0;

        if (!(ss >> x >> y >> level)) {
            throw exception::ParseException{"pic: malformed arguments"};
        }
        std::vector<std::uint32_t> ids;
        std::string idToken;
        while (ss >> idToken) {
            ids.push_back(parseId(idToken));
        }
        for (const auto id : ids) {
            _state.get().setPlayerIncanting(id, true);
            _state.get().broadcastEvent(game::EventType::Incantation,
                                        render::Vector3{static_cast<float>(x), 0.0F, static_cast<float>(y)});
        }
    }

  private:
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network::handlers
