/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TimeUnitSlider
*/

#include "TimeUnitSlider.hpp"

#include <raylib.h>

#include <algorithm>
#include <cmath>

#include "../network/CommandSender.hpp"
#include "Mouse.hpp"
#include "Widgets.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"

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

void TimeUnitSlider::draw(const game::GameState& state, render::Rectangle bounds) {
    static constexpr float kValueBoxWidth = 60.0F;
    static constexpr float kValueBoxSpacing = 6.0F;

    const render::Rectangle valueBoxBounds{bounds.x() + bounds.width() - kValueBoxWidth, bounds.y(), kValueBoxWidth,
                                           bounds.height()};
    const render::Rectangle sliderBounds{bounds.x(), bounds.y(), bounds.width() - kValueBoxWidth - kValueBoxSpacing,
                                         bounds.height()};

    if (!_dragging && Mouse::isLeftButtonPressed() && CheckCollisionPointRec(Mouse::position(), sliderBounds)) {
        _dragging = true;
    }
    if (!_dragging && !_editingValue) {
        _displayedValue = std::clamp(static_cast<int>(state.timeUnit()), kMinValue, kMaxValue);
        _lastSentValue = _displayedValue;
    }

    const float sliderResult = Widgets::slider(sliderBounds, "Time unit", "", static_cast<float>(_displayedValue),
                                               static_cast<float>(kMinValue), static_cast<float>(kMaxValue));
    if (!_editingValue) {
        _displayedValue = std::clamp(static_cast<int>(std::lround(sliderResult)), kMinValue, kMaxValue);
    }

    _displayedValue = Widgets::valueBox(valueBoxBounds, "", _displayedValue, kMinValue, kMaxValue, _editingValue);
    if (!_editingValue) {
        _displayedValue = std::clamp(_displayedValue, kMinValue, kMaxValue);
    }

    if (_dragging && Mouse::isLeftButtonReleased()) {
        _dragging = false;
    }
    if (!_dragging && !_editingValue && consumeValueChange(_displayedValue)) {
        _sender.get().setTimeUnit(_displayedValue);
    }
}

}  // namespace zappy::gui::ui
