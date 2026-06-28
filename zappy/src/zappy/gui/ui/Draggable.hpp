/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Draggable
*/

#pragma once

#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector2.hpp"

namespace zappy::gui::ui {

class Draggable {
  public:
    Draggable() = default;

    [[nodiscard]] render::Rectangle apply(render::Rectangle bounds, float headerHeight);

    [[nodiscard]] bool isOverHeader(render::Vector2 mouse, render::Rectangle bounds, float headerHeight) const;

    [[nodiscard]] render::Rectangle currentBounds(render::Rectangle bounds) const;

  private:
    render::Vector2 _offset;
    render::Vector2 _dragStartMouse;
    render::Vector2 _dragStartOffset;
    bool _dragging{false};
};

}  // namespace zappy::gui::ui
