/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Rectangle tests
*/

#include "zappy/gui/ui/utils/Rectangle.hpp"

#include <gtest/gtest.h>

namespace ui = zappy::gui::ui;

TEST(RectangleTest, DefaultConstructorZeroesFields) {
    const ui::Rectangle rect;
    EXPECT_FLOAT_EQ(rect.x(), 0.0F);
    EXPECT_FLOAT_EQ(rect.y(), 0.0F);
    EXPECT_FLOAT_EQ(rect.width(), 0.0F);
    EXPECT_FLOAT_EQ(rect.height(), 0.0F);
}

TEST(RectangleTest, ConstructorSetsFields) {
    const ui::Rectangle rect{1.0F, 2.0F, 3.0F, 4.0F};
    EXPECT_FLOAT_EQ(rect.x(), 1.0F);
    EXPECT_FLOAT_EQ(rect.y(), 2.0F);
    EXPECT_FLOAT_EQ(rect.width(), 3.0F);
    EXPECT_FLOAT_EQ(rect.height(), 4.0F);
}

TEST(RectangleTest, ConstructsFromNativeType) {
    const ::Rectangle native{.x = 5.0F, .y = 6.0F, .width = 7.0F, .height = 8.0F};
    const ui::Rectangle rect{native};
    EXPECT_FLOAT_EQ(rect.x(), 5.0F);
    EXPECT_FLOAT_EQ(rect.y(), 6.0F);
    EXPECT_FLOAT_EQ(rect.width(), 7.0F);
    EXPECT_FLOAT_EQ(rect.height(), 8.0F);
}

TEST(RectangleTest, ConvertsToNativeType) {
    const ui::Rectangle rect{1.0F, 2.0F, 3.0F, 4.0F};
    const ::Rectangle native = rect;
    EXPECT_FLOAT_EQ(native.x, 1.0F);
    EXPECT_FLOAT_EQ(native.y, 2.0F);
    EXPECT_FLOAT_EQ(native.width, 3.0F);
    EXPECT_FLOAT_EQ(native.height, 4.0F);
}

TEST(RectangleTest, MutableReferenceConversionMutatesOriginal) {
    ui::Rectangle rect{1.0F, 2.0F, 3.0F, 4.0F};
    auto& native = static_cast<::Rectangle&>(rect);
    native.width = 42.0F;
    EXPECT_FLOAT_EQ(rect.width(), 42.0F);
}

TEST(RectangleTest, GetReturnsSameValuesAsAccessors) {
    const ui::Rectangle rect{1.0F, 2.0F, 3.0F, 4.0F};
    const ::Rectangle native = rect.get();
    EXPECT_FLOAT_EQ(native.x, rect.x());
    EXPECT_FLOAT_EQ(native.y, rect.y());
    EXPECT_FLOAT_EQ(native.width, rect.width());
    EXPECT_FLOAT_EQ(native.height, rect.height());
}
