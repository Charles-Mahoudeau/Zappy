/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core code
*/

#include "zappy/server/Core.hpp"

#include <string_view>
#include <vector>

#include "zappy/server/CliParser.hpp"

namespace zappy::server {

void Core::init(const std::vector<std::string_view>& argv) {
    const CliParser::CliParameter params = CliParser::parseArguments(argv);
    CliParser::checkArgumentsValidity(params);

    this->_serv.bind(params.port);
}

}  // namespace zappy::server
