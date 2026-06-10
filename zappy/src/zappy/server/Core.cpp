/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core code
*/

#include "zappy/server/Core.hpp"

#include <string_view>

#include "zappy/server/CliParser.hpp"

namespace zappy::server {

void Core::init(std::span<std::string_view> argv) {
    CliParser parser{argv};
    const CliParser::CliParameters& parameters = parser.parameters();

    this->_serv.bind(parameters.port);
}

}  // namespace zappy::server
