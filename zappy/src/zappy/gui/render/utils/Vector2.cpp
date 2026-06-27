/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** Vector2
*/

#include "Vector2.hpp"

#include <cmath>

namespace zappy::gui::render {
Vector2::Vector2(float x, float y) : _vec2{.x = x, .y = y} {}

Vector2::Vector2(const ::Vector2& vec2) : _vec2{vec2} {}

Vector2& Vector2::operator=(const Vector2& other) {
    _vec2.x = other._vec2.x;
    _vec2.y = other._vec2.y;
    return *this;
}

Vector2& Vector2::operator=(Vector2&& other) noexcept {
    _vec2 = other._vec2;
    other._vec2 = {};
    return *this;
}

Vector2& Vector2::operator+=(const Vector2& other) {
    _vec2.x += other._vec2.x;
    _vec2.y += other._vec2.y;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& other) {
    _vec2.x -= other._vec2.x;
    _vec2.y -= other._vec2.y;
    return *this;
}

Vector2& Vector2::operator*=(float scalar) {
    _vec2.x *= scalar;
    _vec2.y *= scalar;
    return *this;
}

Vector2& Vector2::operator/=(float scalar) {
    if (scalar == 0.0F) {
        return *this;
    }
    _vec2.x /= scalar;
    _vec2.y /= scalar;
    return *this;
}

Vector2 Vector2::operator+(const Vector2& other) const {
    return Vector2{_vec2.x + other._vec2.x, _vec2.y + other._vec2.y};
}

Vector2 Vector2::operator-(const Vector2& other) const {
    return Vector2{_vec2.x - other._vec2.x, _vec2.y - other._vec2.y};
}

Vector2 Vector2::operator*(float scalar) const { return Vector2{_vec2.x * scalar, _vec2.y * scalar}; }

Vector2 Vector2::operator/(float scalar) const {
    if (scalar == 0.0F) {
        return *this;
    }
    return Vector2{_vec2.x / scalar, _vec2.y / scalar};
}

bool Vector2::operator==(const Vector2& other) const { return _vec2.x == other._vec2.x && _vec2.y == other._vec2.y; }

bool Vector2::operator!=(const Vector2& other) const { return !(*this == other); }

float Vector2::length() const { return std::sqrt(lengthSquared()); }

float Vector2::lengthSquared() const { return (_vec2.x * _vec2.x) + (_vec2.y * _vec2.y); }

Vector2 Vector2::normalized() const {
    const float len = length();
    if (len == 0.0F) {
        return Vector2{0.0F, 0.0F};
    }
    return *this / len;
}

float Vector2::x() const { return _vec2.x; }

float Vector2::y() const { return _vec2.y; }

::Vector2 Vector2::get() const { return _vec2; }

Vector2::operator ::Vector2() const { return _vec2; }

Vector2::operator ::Vector2&() { return _vec2; }

}  // namespace zappy::gui::render
