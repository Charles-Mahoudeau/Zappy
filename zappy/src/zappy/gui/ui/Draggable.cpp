/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Draggable
*/

#include "Draggable.hpp"

#include <raylib.h>

#include "Mouse.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"
#include "zappy/gui/render/utils/Vector2.hpp"

namespace zappy::gui::ui {

render::Rectangle Draggable::apply(render::Rectangle bounds, float headerHeight) {
    const render::Rectangle dragged = currentBounds(bounds);
    const render::Rectangle header{dragged.x(), dragged.y(), dragged.width(), headerHeight};
    const render::Vector2 mouse = Mouse::position();

    if (!_dragging && Mouse::isLeftButtonPressed() && CheckCollisionPointRec(mouse, header)) {
        _dragging = true;
        _dragStartMouse = mouse;
        _dragStartOffset = _offset;
    }

    if (_dragging) {
        if (Mouse::isLeftButtonReleased()) {
            _dragging = false;
        } else {
            _offset = render::Vector2{_dragStartOffset.x() + (mouse.x() - _dragStartMouse.x()),
                                      _dragStartOffset.y() + (mouse.y() - _dragStartMouse.y())};
        }
    }

    return currentBounds(bounds);
}

bool Draggable::isOverHeader(render::Vector2 mouse, render::Rectangle bounds, float headerHeight) const {
    const render::Rectangle header{bounds.x() + _offset.x(), bounds.y() + _offset.y(), bounds.width(), headerHeight};
    return CheckCollisionPointRec(mouse, header);
}

render::Rectangle Draggable::currentBounds(render::Rectangle bounds) const {
    return render::Rectangle{bounds.x() + _offset.x(), bounds.y() + _offset.y(), bounds.width(), bounds.height()};
}

}  // namespace zappy::gui::ui
