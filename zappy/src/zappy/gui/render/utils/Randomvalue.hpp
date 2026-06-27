/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Randomvalue
*/

#pragma once

#include <raylib.h>

#include <cmath>
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
    void setEnvelope(float envelope) { _envelope = std::abs(envelope); }

    [[nodiscard]] float value() const { return _value; }
    [[nodiscard]] float envelope() const { return _envelope; }

    [[nodiscard]] float generate() const {
        const float rawEnvelope = std::abs(_value * _envelope);
        const float min = _value - rawEnvelope;
        const float max = _value + rawEnvelope;
        auto [rangeMin, rangeMax] = std::minmax(min, max);
        auto& rng = getRng();

        std::uniform_real_distribution<float> dist{rangeMin, rangeMax};

        return dist(rng);
    }

  private:
    static std::mt19937& getRng() {
        static thread_local std::mt19937 rng{std::random_device{}()};
        return rng;
    }

    float _value{0.0F};
    float _envelope{0.0F};
};
}  // namespace zappy::gui::render
