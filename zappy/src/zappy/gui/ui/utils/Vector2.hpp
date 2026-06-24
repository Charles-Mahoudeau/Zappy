/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Vector2
*/

#pragma once

#include <raylib.h>

namespace zappy::gui::ui {
class Vector2 {
  public:
    Vector2(float x, float y) : _vector{.x = x, .y = y} {}
    Vector2() : _vector{.x = 0.0F, .y = 0.0F} {}
    explicit Vector2(const ::Vector2& vector) : _vector{vector} {}
    Vector2(const Vector2&) = default;
    Vector2& operator=(const Vector2&) = default;
    Vector2(Vector2&&) noexcept = default;
    Vector2& operator=(Vector2&&) noexcept = default;
    ~Vector2() = default;

    [[nodiscard]] ::Vector2 get() const { return _vector; }

    operator ::Vector2() const { return _vector; }
    operator ::Vector2&() { return _vector; }

    [[nodiscard]] float x() const { return _vector.x; }
    [[nodiscard]] float y() const { return _vector.y; }

  private:
    ::Vector2 _vector{};
};
}  // namespace zappy::gui::ui
