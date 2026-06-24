/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** PlayerInfo tests
*/

#include "zappy/gui/ui/PlayerInfo.hpp"

#include <gtest/gtest.h>

#include "zappy/gui/render/utils/Ray.hpp"

namespace ui = zappy::gui::ui;

TEST(PlayerInfoTest, IntersectsPlayerDirectlyBelow) {
    const zappy::gui::render::Ray ray{{2.0F, 5.0F, 3.0F}, {0.0F, -1.0F, 0.0F}};
    EXPECT_TRUE(ui::PlayerInfo::intersectsPlayer(ray, 2.0F, 3.0F, ui::PlayerInfo::kPickRadius));
}

TEST(PlayerInfoTest, MissesPlayerFarAway) {
    const zappy::gui::render::Ray ray{{2.0F, 5.0F, 3.0F}, {0.0F, -1.0F, 0.0F}};
    EXPECT_FALSE(ui::PlayerInfo::intersectsPlayer(ray, 20.0F, 30.0F, ui::PlayerInfo::kPickRadius));
}

TEST(PlayerInfoTest, MissesWhenSphereIsBehindRayOrigin) {
    const zappy::gui::render::Ray ray{{2.0F, -5.0F, 3.0F}, {0.0F, -1.0F, 0.0F}};
    EXPECT_FALSE(ui::PlayerInfo::intersectsPlayer(ray, 2.0F, 3.0F, ui::PlayerInfo::kPickRadius));
}

TEST(PlayerInfoTest, HitsEdgeOfSphereAtRadius) {
    const zappy::gui::render::Ray ray{{2.0F + ui::PlayerInfo::kPickRadius, 5.0F, 3.0F}, {0.0F, -1.0F, 0.0F}};
    EXPECT_TRUE(ui::PlayerInfo::intersectsPlayer(ray, 2.0F, 3.0F, ui::PlayerInfo::kPickRadius));
}

TEST(PlayerInfoTest, MissesJustOutsideRadius) {
    const zappy::gui::render::Ray ray{{2.0F + ui::PlayerInfo::kPickRadius + 0.1F, 5.0F, 3.0F}, {0.0F, -1.0F, 0.0F}};
    EXPECT_FALSE(ui::PlayerInfo::intersectsPlayer(ray, 2.0F, 3.0F, ui::PlayerInfo::kPickRadius));
}
