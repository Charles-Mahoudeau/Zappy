/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core code
*/

#include "zappy/server/Core.hpp"

#include <string_view>
#include <vector>

#include "zappy/server/CliParsing.hpp"

namespace zappy::server {

void Core::init(const std::vector<std::string_view>& argv) {
    const CliParsing::CliParameter params = CliParsing::parseArguments(argv);
    CliParsing::checkArgumentsValidity(params);

    this->_serv.bind(params.port);
}

}  // namespace zappy::server
