/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Draggable
*/

#include "Draggable.hpp"

#include <raylib.h>

#include "Mouse.hpp"

namespace zappy::gui::ui {

Rectangle Draggable::apply(Rectangle bounds, float headerHeight) {
    const Rectangle dragged{bounds.x() + _offset.x(), bounds.y() + _offset.y(), bounds.width(), bounds.height()};
    const Rectangle header{dragged.x(), dragged.y(), dragged.width(), headerHeight};
    const Vector2 mouse = Mouse::position();

    if (!_dragging && Mouse::isLeftButtonPressed() && CheckCollisionPointRec(mouse, header)) {
        _dragging = true;
        _dragStartMouse = mouse;
        _dragStartOffset = _offset;
    }

    if (_dragging) {
        if (Mouse::isLeftButtonReleased()) {
            _dragging = false;
        } else {
            _offset = Vector2{_dragStartOffset.x() + (mouse.x() - _dragStartMouse.x()),
                              _dragStartOffset.y() + (mouse.y() - _dragStartMouse.y())};
        }
    }

    return Rectangle{bounds.x() + _offset.x(), bounds.y() + _offset.y(), bounds.width(), bounds.height()};
}

}  // namespace zappy::gui::ui
