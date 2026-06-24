/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Rectangle
*/

#pragma once

#include <raylib.h>

namespace zappy::gui::ui {
class Rectangle {
  public:
    Rectangle(float x, float y, float width, float height)
        : _rectangle{.x = x, .y = y, .width = width, .height = height} {}
    Rectangle() : _rectangle{.x = 0.0F, .y = 0.0F, .width = 0.0F, .height = 0.0F} {}
    explicit Rectangle(const ::Rectangle& rectangle) : _rectangle{rectangle} {}
    Rectangle(const Rectangle&) = default;
    Rectangle& operator=(const Rectangle&) = default;
    Rectangle(Rectangle&&) noexcept = default;
    Rectangle& operator=(Rectangle&&) noexcept = default;
    ~Rectangle() = default;

    [[nodiscard]] ::Rectangle get() const { return _rectangle; }

    operator ::Rectangle() const { return _rectangle; }
    operator ::Rectangle&() { return _rectangle; }

    [[nodiscard]] float x() const { return _rectangle.x; }
    [[nodiscard]] float y() const { return _rectangle.y; }
    [[nodiscard]] float width() const { return _rectangle.width; }
    [[nodiscard]] float height() const { return _rectangle.height; }

  private:
    ::Rectangle _rectangle{};
};
}  // namespace zappy::gui::ui
