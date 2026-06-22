/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Mouse
*/

#include "Mouse.hpp"

#include <raylib.h>

namespace zappy::gui::ui {

bool Mouse::isLeftButtonReleased() { return IsMouseButtonReleased(MOUSE_BUTTON_LEFT); }

}  // namespace zappy::gui::ui
