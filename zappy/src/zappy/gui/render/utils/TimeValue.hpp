/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TimeValue
*/

#pragma once

#include <raylib.h>

#include "Vector3.hpp"

namespace zappy::gui::render {
template <typename T>
class TimeValue {
  public:
    TimeValue(T init, T inc, float speed = 1.0f) : _value{init}, _increment{inc}, _speed{speed} {}
    TimeValue() = default;
    ~TimeValue() = default;

    [[nodiscard]] T operator() const { return _value; }
    [[nodiscard]] T get() const { return _value; }
    [[nodiscard]] T increment() const { return _increment; }

    void update(float dt) { _value += _increment * dt * _speed; }

  private:
    T _value{};
    T _increment{};
    float _speed{};
};
}  // namespace zappy::gui::render
