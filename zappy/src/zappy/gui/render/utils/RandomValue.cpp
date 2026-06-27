/*
** EPITECH PROJECT, 2026
** projectMirror
** File description:
** RandomValue
*/

#include "RandomValue.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace zappy::gui::render {

RandomValue::RandomValue(float val, float envelope) : _value{val} { setEnvelope(envelope); }

void RandomValue::setValue(float val) { _value = val; }
void RandomValue::setEnvelope(float envelope) { _envelope = std::abs(envelope); }

float RandomValue::value() const { return _value; }
float RandomValue::envelope() const { return _envelope; }

float RandomValue::generate() const {
    if (_envelope == 0.0F) {
        return _value;
    }
    const float rawEnvelope = std::abs(_envelope / 2.0F);
    const float min = _value - rawEnvelope;
    const float max = _value + rawEnvelope;
    auto [rangeMin, rangeMax] = std::minmax<float>(min, max);
    auto& rng = getRng();  // NOSONAR

    std::uniform_real_distribution<float> dist{rangeMin, rangeMax};
    return dist(rng);
}

std::mt19937& RandomValue::getRng() {                              // NOSONAR
    static thread_local std::mt19937 rng{std::random_device{}()};  // NOSONAR
    return rng;
}

}  // namespace zappy::gui::render
