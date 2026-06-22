/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TimeUnitSlider
*/

#include "TimeUnitSlider.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <string>

#include "Mouse.hpp"
#include "Widgets.hpp"

namespace zappy::gui::ui {

TimeUnitSlider::TimeUnitSlider(network::CommandSender& sender, int initialValue)
    : _sender{sender},
      _lastSentValue{std::clamp(initialValue, kMinValue, kMaxValue)},
      _displayedValue{_lastSentValue} {}

bool TimeUnitSlider::consumeValueChange(int newValue) {
    if (newValue == _lastSentValue) {
        return false;
    }
    _lastSentValue = newValue;
    return true;
}

void TimeUnitSlider::draw(Rectangle bounds) {
    const std::string valueLabel = std::format("{}", _displayedValue);
    const float displayed = Widgets::slider(bounds, "Time unit", valueLabel, static_cast<float>(_displayedValue),
                                            static_cast<float>(kMinValue), static_cast<float>(kMaxValue));
    _displayedValue = std::clamp(static_cast<int>(std::lround(displayed)), kMinValue, kMaxValue);

    if (Mouse::isLeftButtonReleased() && consumeValueChange(_displayedValue)) {
        _sender.get().setTimeUnit(_displayedValue);
    }
}

}  // namespace zappy::gui::ui
