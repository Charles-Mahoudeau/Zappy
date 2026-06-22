/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Vec2D
*/

#pragma once

#include <raylib.h>

namespace zappy::gui::render {

class Vec2D {
  public:
    Vec2D(float x, float y);
    ~Vec2D() = default;

    [[nodiscard]] Vec2D& operator=(const Vec2D&);
    [[nodiscard]] Vec2D& operator=(Vec2D&&) noexcept;

    [[nodiscard]] Vec2D& operator+=(const Vec2D&);
    [[nodiscard]] Vec2D& operator-=(const Vec2D&);
    [[nodiscard]] Vec2D& operator*=(float);
    [[nodiscard]] Vec2D& operator/=(float);

    [[nodiscard]] Vec2D operator+(const Vec2D&) const;
    [[nodiscard]] Vec2D operator-(const Vec2D&) const;
    [[nodiscard]] Vec2D operator*(float) const;
    [[nodiscard]] Vec2D operator/(float) const;

    [[nodiscard]] bool operator==(const Vec2D&) const;
    [[nodiscard]] bool operator!=(const Vec2D&) const;

    [[nodiscard]] float length() const;
    [[nodiscard]] float lengthSquared() const;

    [[nodiscard]] Vec2D normalized() const;

    [[nodiscard]] float x() const;
    [[nodiscard]] float y() const;

  private:
    Vector2 _vec2{};
};

}  // namespace zappy::gui::render
