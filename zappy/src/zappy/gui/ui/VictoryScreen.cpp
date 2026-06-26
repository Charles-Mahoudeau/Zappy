/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** VictoryScreen
*/

#include "VictoryScreen.hpp"

#include <format>
#include <string>
#include <string_view>

#include "Widgets.hpp"
#include "zappy/gui/render/utils/Rectangle.hpp"

namespace zappy::gui::ui {

bool VictoryScreen::draw(std::string_view winnerTeam, render::Rectangle screenBounds) {
    static constexpr float kPanelWidth = 360.0F;
    static constexpr float kPanelHeight = 160.0F;
    static constexpr float kTextHeight = 24.0F;
    static constexpr float kTextMarginTop = 16.0F;
    static constexpr float kButtonWidth = 200.0F;
    static constexpr float kButtonHeight = 32.0F;
    static constexpr float kButtonMarginBottom = 16.0F;

    Widgets::overlay(screenBounds);

    const render::Rectangle panelBounds{screenBounds.x() + ((screenBounds.width() - kPanelWidth) / 2.0F),
                                        screenBounds.y() + ((screenBounds.height() - kPanelHeight) / 2.0F), kPanelWidth,
                                        kPanelHeight};
    Widgets::panel(panelBounds, "Victory");

    const std::string text = std::format("{} wins the game!", winnerTeam);
    Widgets::centeredLabel(
        render::Rectangle{panelBounds.x(), panelBounds.y() + Widgets::kPanelHeaderHeight + kTextMarginTop,
                          panelBounds.width(), kTextHeight},
        text);

    const render::Rectangle buttonBounds{panelBounds.x() + ((panelBounds.width() - kButtonWidth) / 2.0F),
                                         panelBounds.y() + panelBounds.height() - kButtonHeight - kButtonMarginBottom,
                                         kButtonWidth, kButtonHeight};
    return Widgets::button(buttonBounds, "Back to game");
}

}  // namespace zappy::gui::ui
