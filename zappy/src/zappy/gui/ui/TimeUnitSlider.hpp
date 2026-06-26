/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TimeUnitSlider
*/

#pragma once

#include <functional>

#include "zappy/gui/network/CommandSender.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"

namespace zappy::gui::game {
class GameState;
}

namespace zappy::gui::ui {

class TimeUnitSlider {
  public:
    explicit TimeUnitSlider(network::CommandSender& sender, int initialValue);
    ~TimeUnitSlider() = default;

    TimeUnitSlider(const TimeUnitSlider&) = delete;
    TimeUnitSlider& operator=(const TimeUnitSlider&) = delete;

    TimeUnitSlider(TimeUnitSlider&&) noexcept = default;
    TimeUnitSlider& operator=(TimeUnitSlider&&) noexcept = default;

    void draw(const game::GameState& state, render::Rectangle bounds);
    bool consumeValueChange(int newValue);

    static constexpr int kMinValue = 1;
    static constexpr int kMaxValue = 1000;

  private:
    std::reference_wrapper<network::CommandSender> _sender;
    int _lastSentValue;
    int _displayedValue;
    bool _dragging{false};
    bool _editingValue{false};
};

}  // namespace zappy::gui::ui
