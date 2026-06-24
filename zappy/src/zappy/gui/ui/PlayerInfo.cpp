/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** PlayerInfo
*/

#include "PlayerInfo.hpp"

#include <raylib.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <format>
#include <string>
#include <string_view>
#include <utility>

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

    const auto it = state.players().find(playerId);
    if (it == state.players().end()) {
        return;
    }
    const auto& player = it->second;

    Widgets::panel(bounds, std::format("Player #{} ({})", playerId, player.team));

    const std::array<std::string, 4> infoRows{
        std::format("Position: ({}, {})", player.x, player.y),
        std::format("Orientation: {}", orientationName(player.orientation)),
        std::format("Level: {}", player.level),
        std::format("Incanting: {}", player.isIncanting ? "yes" : "no"),
    };

    const std::array<std::pair<std::string_view, std::uint32_t>, 7> inventoryRows{{
        {"Food", player.inventory.food},
        {"Linemate", player.inventory.linemate},
        {"Deraumere", player.inventory.deraumere},
        {"Sibur", player.inventory.sibur},
        {"Mendiane", player.inventory.mendiane},
        {"Phiras", player.inventory.phiras},
        {"Thystame", player.inventory.thystame},
    }};

    std::size_t row = 0;
    for (const auto& text : infoRows) {
        ++row;
        const Rectangle rowBounds{bounds.x(), bounds.y() + (static_cast<float>(row) * kRowHeight), bounds.width(),
                                  kRowHeight};
        Widgets::label(rowBounds, text);
    }

    for (const auto& [label, count] : inventoryRows) {
        ++row;
        const std::string text = std::format("{}: {}", label, count);
        const Rectangle rowBounds{bounds.x(), bounds.y() + (static_cast<float>(row) * kRowHeight), bounds.width(),
                                  kRowHeight};
        Widgets::label(rowBounds, text);
    }
}

}  // namespace zappy::gui::ui
