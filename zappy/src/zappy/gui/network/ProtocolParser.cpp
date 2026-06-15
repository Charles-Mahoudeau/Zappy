/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ProtocolParser
*/

#include "zappy/gui/network/ProtocolParser.hpp"

#include <sstream>
#include <string>
#include <string_view>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/HandlerFactory.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network {

ProtocolParser::ProtocolParser(game::GameState& state) : _state{state}, _handlers{HandlerFactory::create(state)} {}

void ProtocolParser::dispatch(std::string_view line) {
    std::istringstream ss{std::string{line}};
    std::string command;
    if (!(ss >> command)) {
        return;
    }
    const auto it = _handlers.find(command);
    if (it == _handlers.end()) {
        return;
    }
    try {
        it->second(ss);
    } catch (const exception::ParseException&) {  // NOLINT(bugprone-empty-catch)
        // malformed protocol lines are ignored, the GUI must not crash on bad server data
    }
}

}  // namespace zappy::gui::network
