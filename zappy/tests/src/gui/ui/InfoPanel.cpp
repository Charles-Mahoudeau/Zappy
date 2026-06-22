/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** InfoPanel tests
*/

#include "zappy/gui/ui/InfoPanel.hpp"

#include <gtest/gtest.h>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/Camera.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace ui = zappy::gui::ui;

namespace {

class InfoPanelTest : public ::testing::Test {
  protected:
    zappy::gui::game::GameState _state;
    zappy::gui::render::Camera _camera{
        zappy::gui::render::Vector3{0.0F, 0.0F, 0.0F}, zappy::gui::render::Vector3{0.0F, 0.0F, 0.0F},
        zappy::gui::render::Vector3{0.0F, 1.0F, 0.0F}, 45.0F, zappy::gui::render::CameraProjection::CAMERA_PERSPECTIVE};
};

TEST_F(InfoPanelTest, DefaultsToLeaderboard) {
    const ui::InfoPanel panel;
    EXPECT_EQ(panel.state(), ui::InfoPanelState::Leaderboard);
}

TEST_F(InfoPanelTest, StaysOnLeaderboardWhenNoClick) {
    ui::InfoPanel panel;
    panel.update(ui::Vector2{0.0F, 0.0F}, false, _camera, _state);
    EXPECT_EQ(panel.state(), ui::InfoPanelState::Leaderboard);
}

TEST_F(InfoPanelTest, ClickOnEmptySpaceStaysOnLeaderboard) {
    ui::InfoPanel panel;
    panel.update(ui::Vector2{0.0F, 0.0F}, true, _camera, _state);
    EXPECT_EQ(panel.state(), ui::InfoPanelState::Leaderboard);
    EXPECT_FALSE(panel.selectedTile().has_value());
    EXPECT_FALSE(panel.selectedPlayerId().has_value());
}

}  // namespace
