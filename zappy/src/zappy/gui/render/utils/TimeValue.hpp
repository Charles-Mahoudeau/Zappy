/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TimeValue
*/

#pragma once

namespace zappy::gui::render {
template <typename T>
class TimeValue {
  public:
    TimeValue(T init, T inc, float speed = 1.0F) : _value{init}, _increment{inc}, _speed{speed} {}

    TimeValue() = default;
    TimeValue(const TimeValue<T>&) = default;
    TimeValue(TimeValue<T>&&) noexcept = default;
    ~TimeValue() = default;

    TimeValue<T>& operator=(const TimeValue<T>&) = default;
    TimeValue<T>& operator=(TimeValue<T>&&) noexcept = default;

    [[nodiscard]] operator T() const { return _value; }
    [[nodiscard]] T get() const { return _value; }
    [[nodiscard]] T increment() const { return _increment; }
    [[nodiscard]] float speed() const { return _speed; }

    void setValue(T val) { _value = val; }
    void setIncrement(T inc) { _increment = inc; }
    void setSpeed(float speed) { _speed = speed; }

    void update(float dt) { _value += (_increment * dt * _speed); }

  private:
    T _value{};
    T _increment{};
    float _speed{1.0F};
};
}  // namespace zappy::gui::render
