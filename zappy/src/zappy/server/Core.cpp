/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core code
*/

#include "zappy/server/Core.hpp"

#include <iostream>
#include <string_view>

#include "zappy/server/CliParsing.hpp"

namespace zappy::server {

void Core::init(const std::vector<std::string_view>& argv) {
    CliParsing::CliParameter params = CliParsing::parseArguments(argv);
    CliParsing::checkArgumentsValidity(params);

    std::cout << params.port << "\n";

    this->_serv.bind(params.port);
}

void Core::run() { std::cout << "run :)\n"; }

}  // namespace zappy::server
