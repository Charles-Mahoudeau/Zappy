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
    TimeValue(const TimeValue<T>& other) : _value{other._value}, _increment{other._increment}, _speed{other._speed} {}
    TimeValue() = default;
    ~TimeValue() = default;

    TimeValue<T>& operator=(const TimeValue<T>& other) {
        _value = other._value;
        _increment = other._increment;
        _speed = other._speed;
        return *this;
    }

    [[nodiscard]] operator T() const { return _value; }
    [[nodiscard]] T get() const { return _value; }
    [[nodiscard]] T increment() const { return _increment; }
    [[nodiscard]] float speed() const { return _speed; }

    void setValue(T val) { _value = val; }
    void setIncrement(T inc) { _increment = inc; }
    void setSpeed(float speed) { _speed = speed; }

    void update(float dt) { _value += _increment * dt * _speed; }

  private:
    T _value{};
    T _increment{};
    float _speed{};
};
}  // namespace zappy::gui::render
