/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** main
*/

#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <span>
#include <string_view>
#include <vector>

#include "zappy/gui/GUI.hpp"
#include "zappy/gui/GuiCliParser.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"

static constexpr std::string_view kUsage = "USAGE: ./zappy_gui -p port -h machine\n";

int main(const int argc, char** argv) {
    const std::span argsView{argv, static_cast<std::size_t>(argc)};
    const std::vector<std::string_view> args{argsView.begin(), argsView.end()};

    if (std::ranges::any_of(args, [](std::string_view a) { return a == "--help"; })) {
        std::cout << kUsage;
        return EXIT_SUCCESS;
    }

    try {
        const zappy::gui::GuiCliParser cli{std::span{args}.subspan(1)};
        return zappy::gui::GUI{}.run(cli);
    } catch (const zappy::exception::InvalidArgument&) {
        std::cerr << kUsage;
        return EXIT_FAILURE;
    } catch (const std::exception& err) {
        std::cerr << err.what() << '\n';
        return EXIT_FAILURE;
    }
}
