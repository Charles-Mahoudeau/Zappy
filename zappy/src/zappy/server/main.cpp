/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <exception>
#include <iostream>
#include <iterator>
#include <string_view>
#include <vector>

#include "zappy/server/Core.hpp"
#include "zappy/shared/exception/Exception.hpp"

int main(const int argc, char** argv) {
    zappy::server::Core core;
    std::vector<std::string_view> arguments(std::next(argv), std::next(argv, argc));

    try {
        core.init(arguments);
    } catch (const zappy::exception::Exception& err) {
        std::cerr << "Error: " << err.what() << "\n";
        return 84;
    } catch (const std::exception& err) {
        std::cerr << "Unknown Error: " << err.what() << "\n";
        return 84;
    }
    return 0;
}
