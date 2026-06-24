/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TileInfo
*/

#include "TileInfo.hpp"

#include <raylib.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "Widgets.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/Camera.hpp"
#include "zappy/gui/render/utils/Ray.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"
#include "zappy/gui/ui/utils/Rectangle.hpp"
#include "zappy/gui/ui/utils/Vector2.hpp"

namespace zappy::gui::ui {

namespace {
constexpr float kGroundPlaneEpsilon = 1e-6F;
}  // namespace

std::optional<std::pair<std::uint32_t, std::uint32_t>> TileInfo::intersectGroundPlane(const render::Ray& ray,
                                                                                      std::size_t mapWidth,
                                                                                      std::size_t mapHeight) {
    const render::Vector3 rayOrigin = ray.position();
    const render::Vector3 rayDirection = ray.direction();

    if (std::fabs(rayDirection.y()) < kGroundPlaneEpsilon) {
        return std::nullopt;
    }

    const float t = -rayOrigin.y() / rayDirection.y();
    if (t < 0.0F) {
        return std::nullopt;
    }

    const float hitX = rayOrigin.x() + (t * rayDirection.x());
    const float hitZ = rayOrigin.z() + (t * rayDirection.z());

    const float tileXf = std::floor(hitX + 0.5F);
    const float tileZf = std::floor(hitZ + 0.5F);
    if (tileXf < 0.0F || tileZf < 0.0F) {
        return std::nullopt;
    }

    const auto tileX = static_cast<std::uint32_t>(tileXf);
    const auto tileY = static_cast<std::uint32_t>(tileZf);
    if (tileX >= mapWidth || tileY >= mapHeight) {
        return std::nullopt;
    }

    return std::pair{tileX, tileY};
}

std::optional<std::pair<std::uint32_t, std::uint32_t>> TileInfo::pick(Vector2 mousePos, const render::Camera& camera,
                                                                      const game::GameState& state) {
    const render::Ray ray{GetMouseRay(mousePos, camera.operator Camera3D())};
    return intersectGroundPlane(ray, state.width(), state.height());
}

void TileInfo::draw(const game::GameState& state, std::uint32_t x, std::uint32_t y, Rectangle bounds) {
    static constexpr float kRowHeight = 24.0F;
    static constexpr float kTextMarginLeft = 8.0F;
    static constexpr float kTextMarginTop = 4.0F;

    Widgets::panel(bounds, std::format("Tile ({}, {})", x, y));

    const game::Resources& resources = state.tile(x, y);
    const std::array<std::pair<std::string_view, std::uint32_t>, 7> rows{{
        {"Food", resources.food},
        {"Linemate", resources.linemate},
        {"Deraumere", resources.deraumere},
        {"Sibur", resources.sibur},
        {"Mendiane", resources.mendiane},
        {"Phiras", resources.phiras},
        {"Thystame", resources.thystame},
    }};

    for (std::size_t i = 0; i < rows.size(); ++i) {
        const auto& [label, count] = rows.at(i);
        const std::string text = std::format("{}: {}", label, count);
        const Rectangle rowBounds{
            bounds.x() + kTextMarginLeft,
            bounds.y() + Widgets::kPanelHeaderHeight + kTextMarginTop + (static_cast<float>(i) * kRowHeight),
            bounds.width(), kRowHeight};
        Widgets::label(rowBounds, text);
    }
}

}  // namespace zappy::gui::ui
