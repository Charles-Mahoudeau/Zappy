/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** PlayerInfo
*/

#include "PlayerInfo.hpp"

#include <raylib.h>

#include <cmath>
#include <format>
#include <initializer_list>
#include <string>
#include <string_view>

#include "Widgets.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/Camera.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::ui {

namespace {
std::string_view orientationName(game::Orientation orientation) {
    using enum game::Orientation;
    switch (orientation) {
        case North:
            return "North";
        case East:
            return "East";
        case South:
            return "South";
        case West:
            return "West";
    }
    return "Unknown";
}
}  // namespace

bool PlayerInfo::intersectsPlayer(const render::Ray& ray, float playerX, float playerY, float radius) {
    const render::Vector3 origin = ray.position();
    const render::Vector3 direction = ray.direction();

    const float ocx = origin.x() - playerX;
    const float ocy = origin.y() - kPickHeight;
    const float ocz = origin.z() - playerY;

    const float a = (direction.x() * direction.x()) + (direction.y() * direction.y()) + (direction.z() * direction.z());
    const float b = 2.0F * ((ocx * direction.x()) + (ocy * direction.y()) + (ocz * direction.z()));
    const float c = (ocx * ocx) + (ocy * ocy) + (ocz * ocz) - (radius * radius);

    const float discriminant = (b * b) - (4.0F * a * c);
    if (discriminant < 0.0F) {
        return false;
    }

    const float sqrtDiscriminant = std::sqrt(discriminant);
    const float t0 = (-b - sqrtDiscriminant) / (2.0F * a);
    const float t1 = (-b + sqrtDiscriminant) / (2.0F * a);
    return t0 >= 0.0F || t1 >= 0.0F;
}

std::optional<std::uint32_t> PlayerInfo::pick(Vector2 mousePos, const render::Camera& camera,
                                              const game::GameState& state) {
    const render::Ray ray{GetMouseRay(mousePos, camera.operator Camera3D())};

    for (const auto& [id, player] : state.players()) {
        if (intersectsPlayer(ray, static_cast<float>(player.x), static_cast<float>(player.y), kPickRadius)) {
            return id;
        }
    }
    return std::nullopt;
}

void PlayerInfo::draw(const game::GameState& state, std::uint32_t playerId, Rectangle bounds) {
    static constexpr float kRowHeight = 24.0F;
    static constexpr float kTextMarginLeft = 8.0F;
    static constexpr float kTextMarginTop = 4.0F;
    static constexpr float kSectionGap = 8.0F;
    static constexpr float kSectionMargin = 8.0F;

    const auto it = state.players().find(playerId);
    if (it == state.players().end()) {
        return;
    }
    const auto& player = it->second;

    Widgets::panel(bounds, std::format("Player #{} ({})", playerId, player.team));

    float y = bounds.y() + Widgets::kPanelHeaderHeight + kTextMarginTop;
    const float sectionX = bounds.x() + kSectionMargin;
    const float sectionWidth = bounds.width() - (2.0F * kSectionMargin);

    const auto drawSection = [&](std::string_view title, std::initializer_list<std::string> lines) {
        const float sectionHeight =
            Widgets::kPanelHeaderHeight + kTextMarginTop + (static_cast<float>(lines.size()) * kRowHeight);
        const Rectangle sectionBounds{sectionX, y, sectionWidth, sectionHeight};
        Widgets::panel(sectionBounds, title);

        float rowY = sectionBounds.y() + Widgets::kPanelHeaderHeight + kTextMarginTop;
        for (const auto& line : lines) {
            Widgets::label(Rectangle{sectionBounds.x() + kTextMarginLeft, rowY,
                                     sectionBounds.width() - (2.0F * kTextMarginLeft), kRowHeight},
                           line);
            rowY += kRowHeight;
        }
        y += sectionHeight + kSectionGap;
    };

    drawSection("Placement", {
                                 std::format("Position: ({}, {})", player.x, player.y),
                                 std::format("Orientation: {}", orientationName(player.orientation)),
                             });

    drawSection("State", {
                             std::format("Level: {}", player.level),
                             std::format("Incanting: {}", player.isIncanting ? "yes" : "no"),
                         });

    drawSection("Inventory", {
                                 std::format("Food: {}", player.inventory.food),
                                 std::format("Linemate: {}", player.inventory.linemate),
                                 std::format("Deraumere: {}", player.inventory.deraumere),
                                 std::format("Sibur: {}", player.inventory.sibur),
                                 std::format("Mendiane: {}", player.inventory.mendiane),
                                 std::format("Phiras: {}", player.inventory.phiras),
                                 std::format("Thystame: {}", player.inventory.thystame),
                             });
}

}  // namespace zappy::gui::ui
