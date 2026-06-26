/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ChatPanel tests
*/

#include "zappy/gui/ui/ChatPanel.hpp"

#include <gtest/gtest.h>

#include "zappy/gui/render/utils/Rectangle.hpp"

namespace ui = zappy::gui::ui;
namespace render = zappy::gui::render;

TEST(ChatPanelTest, ContentRectIsEmptyForNoMessages) {
    const render::Rectangle bounds{0.0F, 0.0F, 320.0F, 180.0F};
    const render::Rectangle content = ui::ChatPanel::contentRect(bounds, 0);
    EXPECT_FLOAT_EQ(content.width(), 320.0F);
    EXPECT_FLOAT_EQ(content.height(), 0.0F);
}

TEST(ChatPanelTest, ContentRectGrowsWithMessageCount) {
    const render::Rectangle bounds{0.0F, 0.0F, 320.0F, 180.0F};
    const render::Rectangle oneMessage = ui::ChatPanel::contentRect(bounds, 1);
    const render::Rectangle fiveMessages = ui::ChatPanel::contentRect(bounds, 5);
    EXPECT_GT(fiveMessages.height(), oneMessage.height());
    EXPECT_FLOAT_EQ(fiveMessages.height(), oneMessage.height() * 5.0F);
}

TEST(ChatPanelTest, NoAutoScrollResetWhenMessageCountUnchanged) {
    ui::ChatPanel panel;
    EXPECT_FALSE(panel.consumeAutoScrollReset(0));
    EXPECT_FALSE(panel.consumeAutoScrollReset(0));
}

TEST(ChatPanelTest, AutoScrollResetsExactlyOncePerNewMessageCount) {
    ui::ChatPanel panel;
    EXPECT_TRUE(panel.consumeAutoScrollReset(1));
    EXPECT_FALSE(panel.consumeAutoScrollReset(1));
    EXPECT_TRUE(panel.consumeAutoScrollReset(2));
    EXPECT_FALSE(panel.consumeAutoScrollReset(2));
}

TEST(ChatPanelTest, AutoScrollResetsOnMessageCountDecrease) {
    ui::ChatPanel panel;
    EXPECT_TRUE(panel.consumeAutoScrollReset(5));
    EXPECT_TRUE(panel.consumeAutoScrollReset(3));
}
