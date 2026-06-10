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

#include "zappy/gui/display/Window.hpp"

int main(const int argc, char** argv) {
    const std::span argsView{argv, static_cast<std::size_t>(argc)};
    const std::vector<std::string_view> args{argsView.begin(), argsView.end()};

    zappy::gui::display::Window window{1600, 900, "Zappy"};
    window.setTargetFPS(60);

    while (!window.shouldClose()) {
        window.beginFrame();
        window.endFrame();
    }
    return 0;
}
