/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ParseHelpers
*/

#pragma once

#include <cstdint>
#include <exception>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/shared/exception/ParseException.hpp"

namespace zappy::gui::network::handlers {

inline std::int32_t parseId(std::string_view token) {
    if (token.size() < 2 || token.at(0) != '#') {
        throw exception::ParseException{"expected #-prefixed id"};
    }
    try {
        const long value = std::stol(std::string{token.substr(1)});
        if (value > std::numeric_limits<std::int32_t>::max()) {
            throw exception::ParseException{"id value out of int32 range"};
        }
        return static_cast<std::int32_t>(value);
    } catch (const exception::ParseException&) {
        throw;
    } catch (const std::exception&) {
        throw exception::ParseException{"invalid id value"};
    }
}

inline game::Orientation parseOrientation(std::int32_t value) {
    if (value < 1 || value > 4) {
        throw exception::ParseException{"orientation must be between 1 and 4"};
    }
    return static_cast<game::Orientation>(value);
}

inline game::Resources parseResources(std::istringstream& ss) {
    game::Resources res;
    if (!(ss >> res.food >> res.linemate >> res.deraumere >> res.sibur >> res.mendiane >> res.phiras >> res.thystame)) {
        throw exception::ParseException{"malformed resource quantities"};
    }
    return res;
}

}  // namespace zappy::gui::network::handlers
