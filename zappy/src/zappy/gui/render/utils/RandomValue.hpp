/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** RandomValue
*/

#pragma once

#include <algorithm>
#include <cmath>
#include <random>

namespace zappy::gui::render {
class RandomValue {
  public:
    RandomValue(float val, float envelope);
    RandomValue(const RandomValue&) = default;
    RandomValue(RandomValue&&) noexcept = default;
    RandomValue& operator=(const RandomValue&) = default;
    RandomValue& operator=(RandomValue&&) noexcept = default;
    ~RandomValue() = default;

    void setValue(float val);
    void setEnvelope(float envelope);

    [[nodiscard]] float value() const;
    [[nodiscard]] float envelope() const;

    [[nodiscard]] float generate() const;

  private:
    static std::mt19937& getRng();  // NOSONAR - mt19937 is sufficient for particle VFX, not used for security

    float _value{0.0F};
    float _envelope{0.0F};
};
}  // namespace zappy::gui::render
