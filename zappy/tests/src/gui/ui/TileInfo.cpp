/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TileInfo tests
*/

#include "zappy/gui/ui/TileInfo.hpp"

#include <gtest/gtest.h>

#include "zappy/gui/render/utils/Ray.hpp"

namespace ui = zappy::gui::ui;

TEST(TileInfoTest, IntersectsValidTileDirectlyBelow) {
    const zappy::gui::render::Ray ray{{2.0F, 5.0F, 3.0F}, {0.0F, -1.0F, 0.0F}};
    const auto hit = ui::TileInfo::intersectGroundPlane(ray, 10, 10);
    ASSERT_TRUE(hit.has_value());
    const auto [tileX, tileY] = hit.value();  // NOLINT
    EXPECT_EQ(tileX, 2U);
    EXPECT_EQ(tileY, 3U);
}

TEST(TileInfoTest, MissesWhenRayIsParallelToGround) {
    const zappy::gui::render::Ray ray{{0.0F, 5.0F, 0.0F}, {1.0F, 0.0F, 0.0F}};
    EXPECT_FALSE(ui::TileInfo::intersectGroundPlane(ray, 10, 10).has_value());
}

TEST(TileInfoTest, MissesWhenGroundPlaneIsBehindRayOrigin) {
    const zappy::gui::render::Ray ray{{0.0F, -5.0F, 0.0F}, {0.0F, -1.0F, 0.0F}};
    EXPECT_FALSE(ui::TileInfo::intersectGroundPlane(ray, 10, 10).has_value());
}

TEST(TileInfoTest, RejectsHitOutsideMapBoundsPositiveSide) {
    const zappy::gui::render::Ray ray{{50.0F, 5.0F, 50.0F}, {0.0F, -1.0F, 0.0F}};
    EXPECT_FALSE(ui::TileInfo::intersectGroundPlane(ray, 10, 10).has_value());
}

TEST(TileInfoTest, RejectsHitOutsideMapBoundsNegativeSide) {
    const zappy::gui::render::Ray ray{{-5.0F, 5.0F, -5.0F}, {0.0F, -1.0F, 0.0F}};
    EXPECT_FALSE(ui::TileInfo::intersectGroundPlane(ray, 10, 10).has_value());
}

TEST(TileInfoTest, AcceptsHitOnTileZeroZero) {
    const zappy::gui::render::Ray ray{{0.0F, 5.0F, 0.0F}, {0.0F, -1.0F, 0.0F}};
    const auto hit = ui::TileInfo::intersectGroundPlane(ray, 10, 10);
    ASSERT_TRUE(hit.has_value());
    const auto [tileX, tileY] = hit.value();  // NOLINT
    EXPECT_EQ(tileX, 0U);
    EXPECT_EQ(tileY, 0U);
}
