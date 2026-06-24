/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiTheme tests
*/

#include "zappy/gui/ui/GuiTheme.hpp"

#include <gtest/gtest.h>

#include <type_traits>

using zappy::gui::ui::GuiTheme;

TEST(GuiThemeTest, IsMoveOnly) {
    EXPECT_FALSE(std::is_copy_constructible_v<GuiTheme>);
    EXPECT_FALSE(std::is_copy_assignable_v<GuiTheme>);
    EXPECT_TRUE(std::is_move_constructible_v<GuiTheme>);
    EXPECT_TRUE(std::is_move_assignable_v<GuiTheme>);
}

TEST(GuiThemeTest, KeepsDefaultStyleWhenStyleFileMissing) { EXPECT_NO_THROW(GuiTheme{"this/path/does/not/exist.rgs"}); }
