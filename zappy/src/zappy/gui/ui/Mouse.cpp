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

bool Mouse::isLeftButtonPressed() { return IsMouseButtonPressed(MOUSE_BUTTON_LEFT); }

Vector2 Mouse::position() { return Vector2{GetMousePosition()}; }

}  // namespace zappy::gui::ui
