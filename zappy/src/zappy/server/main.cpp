/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <cstddef>
#include <span>
#include <string_view>
#include <vector>

#include "Demo.hpp"
#include "zappy/shared/Demo.hpp"

int main(const int argc, char** argv) {
    const std::span argsView{argv, static_cast<std::size_t>(argc)};
    const std::vector<std::string_view> args{argsView.begin(), argsView.end()};

    zappy::server::hello();
    zappy::shared::hello();
    return 0;
}
