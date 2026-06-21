/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Vector3
*/

#pragma once

#include <raylib.h>

namespace zappy::gui::render {
class Vector3 {
  public:
    Vector3(float x, float y, float z) : _vector{.x = x, .y = y, .z = z} {}
    Vector3() : _vector{.x = 0.0F, .y = 0.0F, .z = 0.0F} {}
    Vector3(const ::Vector3& vector) : _vector{vector} {}
    Vector3(const Vector3&) = default;
    Vector3& operator=(const Vector3&) = default;
    Vector3(Vector3&&) noexcept = default;
    Vector3& operator=(Vector3&&) noexcept = default;
    ~Vector3() = default;

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
