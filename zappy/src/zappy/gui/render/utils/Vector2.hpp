/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Vector2
*/

#pragma once

#include <raylib.h>

namespace zappy::gui::render {

class Vector2 {
  public:
    Vector2() = default;
    Vector2(float x, float y);
    explicit Vector2(const ::Vector2& vec2);
    ~Vector2() = default;

    Vector2(const Vector2&) = default;
    Vector2(Vector2&&) noexcept = default;

    Vector2& operator=(const Vector2&);
    Vector2& operator=(Vector2&&) noexcept;

    Vector2& operator+=(const Vector2&);
    Vector2& operator-=(const Vector2&);
    Vector2& operator*=(float);
    Vector2& operator/=(float);

    [[nodiscard]] Vector2 operator+(const Vector2&) const;
    [[nodiscard]] Vector2 operator-(const Vector2&) const;
    [[nodiscard]] Vector2 operator*(float) const;
    [[nodiscard]] Vector2 operator/(float) const;

    [[nodiscard]] bool operator==(const Vector2&) const;
    [[nodiscard]] bool operator!=(const Vector2&) const;

    [[nodiscard]] float length() const;
    [[nodiscard]] float lengthSquared() const;

    [[nodiscard]] Vector2 normalized() const;

    [[nodiscard]] float x() const;
    [[nodiscard]] float y() const;

    [[nodiscard]] ::Vector2 get() const;
    [[nodiscard]] operator ::Vector2() const;
    [[nodiscard]] operator ::Vector2&();

  private:
    ::Vector2 _vec2{};
};

}  // namespace zappy::gui::render
