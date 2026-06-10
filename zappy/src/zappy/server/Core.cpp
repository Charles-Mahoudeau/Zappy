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
    const CliParser::CliParameters params = CliParser::parseArguments(argv);
    CliParser::ensureValidArguments(params);

    this->_serv.bind(params.port);
}

}  // namespace zappy::server
