/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Vector2 tests
*/

#include "zappy/gui/render/utils/Vector2.hpp"

#include <gtest/gtest.h>

#include "raylib.h"

namespace render = zappy::gui::render;

TEST(Vector2Test, DefaultConstructorZeroesFields) {
    const render::Vector2 vector;
    EXPECT_FLOAT_EQ(vector.x(), 0.0F);
    EXPECT_FLOAT_EQ(vector.y(), 0.0F);
}

TEST(Vector2Test, ConstructorSetsFields) {
    const render::Vector2 vector{1.5F, -2.5F};
    EXPECT_FLOAT_EQ(vector.x(), 1.5F);
    EXPECT_FLOAT_EQ(vector.y(), -2.5F);
}

TEST(Vector2Test, ConstructsFromNativeType) {
    const ::Vector2 native{.x = 3.0F, .y = 4.0F};
    const render::Vector2 vector{native};
    EXPECT_FLOAT_EQ(vector.x(), 3.0F);
    EXPECT_FLOAT_EQ(vector.y(), 4.0F);
}

TEST(Vector2Test, ConvertsToNativeType) {
    const render::Vector2 vector{1.0F, 2.0F};
    const ::Vector2 native = vector;
    EXPECT_FLOAT_EQ(native.x, 1.0F);
    EXPECT_FLOAT_EQ(native.y, 2.0F);
}

TEST(Vector2Test, MutableReferenceConversionMutatesOriginal) {
    render::Vector2 vector{1.0F, 2.0F};
    auto& native = static_cast<::Vector2&>(vector);
    native.x = 9.0F;
    EXPECT_FLOAT_EQ(vector.x(), 9.0F);
}

TEST(Vector2Test, GetReturnsSameValuesAsAccessors) {
    const render::Vector2 vector{5.0F, 6.0F};
    const ::Vector2 native = vector.get();
    EXPECT_FLOAT_EQ(native.x, vector.x());
    EXPECT_FLOAT_EQ(native.y, vector.y());
}
