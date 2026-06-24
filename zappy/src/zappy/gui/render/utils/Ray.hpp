/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Ray
*/

#pragma once

#include <raylib.h>

#include "Vector3.hpp"

namespace zappy::gui::render {
class Ray {
  public:
    Ray(Vector3 position, Vector3 direction) : _ray{.position = position, .direction = direction} {}
    Ray() = default;
    explicit Ray(const ::Ray& ray) : _ray{ray} {}
    Ray(const Ray&) = default;
    Ray& operator=(const Ray&) = default;
    Ray(Ray&&) noexcept = default;
    Ray& operator=(Ray&&) noexcept = default;
    ~Ray() = default;

    [[nodiscard]] ::Ray get() const { return _ray; }

    operator ::Ray() const { return _ray; }
    operator ::Ray&() { return _ray; }

    [[nodiscard]] Vector3 position() const { return Vector3{_ray.position}; }
    [[nodiscard]] Vector3 direction() const { return Vector3{_ray.direction}; }

  private:
    ::Ray _ray{};
};
}  // namespace zappy::gui::render
