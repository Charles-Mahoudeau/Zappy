/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Mouse
*/

#pragma once

#include "utils/Vector2.hpp"

namespace zappy::gui::ui {

class Mouse {
  public:
    Mouse() = delete;

    static bool isLeftButtonReleased();
    static bool isLeftButtonPressed();
    static Vector2 position();
    static float scrollDelta();
};

}  // namespace zappy::gui::ui
