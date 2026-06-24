/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TimeUnitSlider
*/

#pragma once

#include <functional>

#include "utils/Rectangle.hpp"
#include "zappy/gui/network/CommandSender.hpp"

namespace zappy::gui::ui {

class TimeUnitSlider {
  public:
    explicit TimeUnitSlider(network::CommandSender& sender, int initialValue);
    ~TimeUnitSlider() = default;

    TimeUnitSlider(const TimeUnitSlider&) = delete;
    TimeUnitSlider& operator=(const TimeUnitSlider&) = delete;

    TimeUnitSlider(TimeUnitSlider&&) noexcept = default;
    TimeUnitSlider& operator=(TimeUnitSlider&&) noexcept = default;

    void draw(Rectangle bounds);
    bool consumeValueChange(int newValue);

    static constexpr int kMinValue = 1;
    static constexpr int kMaxValue = 100;

  private:
    std::reference_wrapper<network::CommandSender> _sender;
    int _lastSentValue;
    int _displayedValue;
};

}  // namespace zappy::gui::ui
