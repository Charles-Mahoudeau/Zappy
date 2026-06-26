/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Randomvalue
*/

#pragma once

#include <raylib.h>

#include <random>

namespace zappy::gui::render {
class RandomValue {
  public:
    RandomValue(float val, float envelope) : _value{val} { setEnvelope(envelope); }
    RandomValue(const RandomValue&) = default;
    RandomValue(RandomValue&&) noexcept = default;
    RandomValue& operator=(const RandomValue&) = default;
    RandomValue& operator=(RandomValue&&) noexcept = default;
    ~RandomValue() = default;

    void setValue(float val) { _value = val; }
    void setEnvelope(float envelope) {
        float env = envelope > 1.0F ? 1.0F : envelope;

        _envelope = env;
    }

    [[nodiscard]] float value() const { return _value; }
    [[nodiscard]] float envelope() const { return _envelope; }

    [[nodiscard]] operator float() const {
        float min = _value * (1.0F - _envelope);
        float max = _value * (1.0F + _envelope);
        static std::mt19937 rng{std::random_device{}()};
        std::uniform_real_distribution<float> dist{min, max};

        return dist(rng);
    }

  private:
    float _value{0.0F};
    float _envelope{0.0F};
};
}  // namespace zappy::gui::render
