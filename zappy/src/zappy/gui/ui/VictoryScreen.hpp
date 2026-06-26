/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** VictoryScreen
*/

#pragma once

#include <string_view>

#include "zappy/gui/render/utils/Rectangle.hpp"

namespace zappy::gui::ui {

class VictoryScreen {
  public:
    VictoryScreen() = delete;

    [[nodiscard]] static bool draw(std::string_view winnerTeam, render::Rectangle screenBounds);
};

}  // namespace zappy::gui::ui
