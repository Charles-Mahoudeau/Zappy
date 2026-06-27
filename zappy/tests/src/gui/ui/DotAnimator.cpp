/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** DotAnimator tests
*/

#include "zappy/gui/ui/DotAnimator.hpp"

#include <gtest/gtest.h>

namespace ui = zappy::gui::ui;

namespace {

TEST(DotAnimatorTest, StartsWithOneDot) {
    const ui::DotAnimator dots;
    EXPECT_EQ(dots.text("Loading"), "Loading.");
}

TEST(DotAnimatorTest, AdvanceCyclesThroughMaxDots) {
    ui::DotAnimator dots{3};
    dots.advance();
    EXPECT_EQ(dots.text("Loading"), "Loading..");
    dots.advance();
    EXPECT_EQ(dots.text("Loading"), "Loading...");
    dots.advance();
    EXPECT_EQ(dots.text("Loading"), "Loading.");
}

TEST(DotAnimatorTest, UpdateDoesNotAdvanceBeforeInterval) {
    ui::DotAnimator dots;
    dots.update(0.1F);
    EXPECT_EQ(dots.text(""), ".");
}

TEST(DotAnimatorTest, UpdateAdvancesOnceEnoughTimeAccumulates) {
    ui::DotAnimator dots;
    dots.update(0.5F);
    EXPECT_EQ(dots.text(""), "..");
}

TEST(DotAnimatorTest, UpdateCarriesOverLeftoverTime) {
    ui::DotAnimator dots;
    dots.update(0.3F);
    dots.update(0.3F);
    EXPECT_EQ(dots.text(""), "..");
}

}  // namespace
