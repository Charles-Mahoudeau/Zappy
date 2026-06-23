/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** Vec2D
*/

#include "Vec2D.hpp"

#include <raylib.h>

#include <cmath>

namespace zappy::gui::render {
Vec2D::Vec2D(float x, float y) : _vec2{.x = x, .y = y} {}

Vec2D& Vec2D::operator=(const Vec2D& other) {
    _vec2.x = other._vec2.x;
    _vec2.y = other._vec2.y;
    return *this;
}

Vec2D& Vec2D::operator=(Vec2D&& other) noexcept {
    _vec2 = other._vec2;
    other._vec2 = {};
    return *this;
}

Vec2D& Vec2D::operator+=(const Vec2D& other) {
    _vec2.x += other._vec2.x;
    _vec2.y += other._vec2.y;
    return *this;
}

Vec2D& Vec2D::operator-=(const Vec2D& other) {
    _vec2.x -= other._vec2.x;
    _vec2.y -= other._vec2.y;
    return *this;
}

Vec2D& Vec2D::operator*=(float scalar) {
    _vec2.x *= scalar;
    _vec2.y *= scalar;
    return *this;
}

Vec2D& Vec2D::operator/=(float scalar) {
    if (scalar == 0) {
        return *this;
    }
    _vec2.x /= scalar;
    _vec2.y /= scalar;
    return *this;
}

Vec2D Vec2D::operator+(const Vec2D& other) const { return Vec2D{_vec2.x + other._vec2.x, _vec2.y + other._vec2.y}; }

Vec2D Vec2D::operator-(const Vec2D& other) const { return Vec2D{_vec2.x - other._vec2.x, _vec2.y - other._vec2.y}; }

Vec2D Vec2D::operator*(float scalar) const { return Vec2D{_vec2.x * scalar, _vec2.y * scalar}; }

Vec2D Vec2D::operator/(float scalar) const {
    if (scalar == 0) {
        return *this;
    }
    return Vec2D{_vec2.x / scalar, _vec2.y / scalar};
}

bool Vec2D::operator==(const Vec2D& other) const { return _vec2.x == other._vec2.x && _vec2.y == other._vec2.y; }

bool Vec2D::operator!=(const Vec2D& other) const { return !(*this == other); }

float Vec2D::length() const { return std::sqrt(lengthSquared()); }

float Vec2D::lengthSquared() const { return (_vec2.x * _vec2.x) + (_vec2.y * _vec2.y); }

Vec2D Vec2D::normalized() const {
    const float len = length();
    if (len == 0) {
        return Vec2D{0, 0};
    }
    return *this / len;
}

float Vec2D::x() const { return _vec2.x; }

float Vec2D::y() const { return _vec2.y; }
}  // namespace zappy::gui::render
