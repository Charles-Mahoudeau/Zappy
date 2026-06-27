/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Vector3
*/

#pragma once

#include <raylib.h>

#include <cmath>

namespace zappy::gui::render {
class Vector3 {
  public:
    Vector3(float x, float y, float z) : _vector{.x = x, .y = y, .z = z} {}
    Vector3() : _vector{.x = 0.0F, .y = 0.0F, .z = 0.0F} {}
    explicit Vector3(const ::Vector3& vector) : _vector{vector} {}
    Vector3(const Vector3&) = default;
    Vector3& operator=(const Vector3&) = default;
    Vector3(Vector3&&) noexcept = default;
    Vector3& operator=(Vector3&&) noexcept = default;
    ~Vector3() = default;

    [[nodiscard]] Vector3 operator+(const Vector3& other) const {
        return Vector3{_vector.x + other._vector.x, _vector.y + other._vector.y, _vector.z + other._vector.z};
    }
    [[nodiscard]] Vector3 operator-(const Vector3& other) const {
        return Vector3{_vector.x - other._vector.x, _vector.y - other._vector.y, _vector.z - other._vector.z};
    }
    [[nodiscard]] Vector3 operator*(float scalar) const {
        return Vector3{_vector.x * scalar, _vector.y * scalar, _vector.z * scalar};
    }
    [[nodiscard]] Vector3 operator/(float scalar) const {
        if (scalar == 0.0F) {
            return *this;
        }
        return Vector3{_vector.x / scalar, _vector.y / scalar, _vector.z / scalar};
    }

    Vector3& operator+=(const Vector3& other) {
        _vector.x += other._vector.x;
        _vector.y += other._vector.y;
        _vector.z += other._vector.z;
        return *this;
    }
    Vector3& operator-=(const Vector3& other) {
        _vector.x -= other._vector.x;
        _vector.y -= other._vector.y;
        _vector.z -= other._vector.z;
        return *this;
    }
    Vector3& operator*=(float scalar) {
        _vector.x *= scalar;
        _vector.y *= scalar;
        _vector.z *= scalar;
        return *this;
    }
    Vector3& operator/=(float scalar) {
        if (scalar == 0.0F) {
            return *this;
        }
        _vector.x /= scalar;
        _vector.y /= scalar;
        _vector.z /= scalar;
        return *this;
    }

    [[nodiscard]] bool operator==(const Vector3& other) const {
        return (_vector.x == other._vector.x && _vector.y == other._vector.y && _vector.z == other._vector.z);
    }
    [[nodiscard]] bool operator!=(const Vector3& other) const { return !(*this == other); }

    [[nodiscard]] float length() const {
        return std::sqrt((_vector.x * _vector.x) + (_vector.y * _vector.y) + (_vector.z * _vector.z));
    }
    [[nodiscard]] float lengthSquared() const {
        return (_vector.x * _vector.x) + (_vector.y * _vector.y) + (_vector.z * _vector.z);
    }
    [[nodiscard]] Vector3 normalized() const {
        const float len = length();
        if (len == 0.0F) {
            return *this;
        }
        return (*this / len);
    }

    [[nodiscard]] Vector3 cross(const Vector3& other) const {
        return Vector3{(_vector.y * other._vector.z) - (_vector.z * other._vector.y),
                       (_vector.z * other._vector.x) - (_vector.x * other._vector.z),
                       (_vector.x * other._vector.y) - (_vector.y * other._vector.x)};
    }

    [[nodiscard]] ::Vector3 get() const { return _vector; }

    operator ::Vector3() const { return _vector; }
    operator ::Vector3&() { return _vector; }

    [[nodiscard]] float x() const { return _vector.x; }
    [[nodiscard]] float y() const { return _vector.y; }
    [[nodiscard]] float z() const { return _vector.z; }

  protected:
  private:
    ::Vector3 _vector{};
};
}  // namespace zappy::gui::render
