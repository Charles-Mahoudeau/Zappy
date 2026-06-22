/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TimeUnitSlider tests
*/

#include "zappy/gui/ui/TimeUnitSlider.hpp"

#include <gtest/gtest.h>

#include "zappy/gui/network/CommandSender.hpp"
#include "zappy/shared/network/BufferedClient.hpp"

namespace ui = zappy::gui::ui;

namespace {

class TimeUnitSliderTest : public ::testing::Test {
  public:
    zappy::network::BufferedClient _buffer;
    zappy::gui::network::CommandSender _sender{_buffer};
};

TEST_F(TimeUnitSliderTest, NoChangeWhenValueUnchanged) {
    ui::TimeUnitSlider slider{_sender, 10};
    EXPECT_FALSE(slider.consumeValueChange(10));
    EXPECT_FALSE(slider.consumeValueChange(10));
}

TEST_F(TimeUnitSliderTest, ChangesExactlyOncePerNewValue) {
    ui::TimeUnitSlider slider{_sender, 10};
    EXPECT_TRUE(slider.consumeValueChange(20));
    EXPECT_FALSE(slider.consumeValueChange(20));
    EXPECT_TRUE(slider.consumeValueChange(30));
    EXPECT_FALSE(slider.consumeValueChange(30));
}

TEST_F(TimeUnitSliderTest, ChangeDetectedOnDecrease) {
    ui::TimeUnitSlider slider{_sender, 50};
    EXPECT_TRUE(slider.consumeValueChange(10));
}

TEST_F(TimeUnitSliderTest, InitialValueIsClampedToValidRange) {
    ui::TimeUnitSlider tooLow{_sender, 0};
    EXPECT_FALSE(tooLow.consumeValueChange(ui::TimeUnitSlider::kMinValue));

    ui::TimeUnitSlider tooHigh{_sender, 1000};
    EXPECT_FALSE(tooHigh.consumeValueChange(ui::TimeUnitSlider::kMaxValue));
}

}  // namespace
