/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Draggable
*/

#pragma once

#include "utils/Rectangle.hpp"
#include "utils/Vector2.hpp"

namespace zappy::gui::ui {

class Draggable {
  public:
    Draggable() = default;

    [[nodiscard]] Rectangle apply(Rectangle bounds, float headerHeight);

    [[nodiscard]] bool isOverHeader(Vector2 mouse, Rectangle bounds, float headerHeight) const;

  private:
    Vector2 _offset;
    Vector2 _dragStartMouse;
    Vector2 _dragStartOffset;
    bool _dragging{false};
};

}  // namespace zappy::gui::ui
