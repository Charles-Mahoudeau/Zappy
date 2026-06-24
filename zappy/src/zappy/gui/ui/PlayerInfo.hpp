/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** PlayerInfo
*/

#pragma once

#include <cstdint>
#include <optional>

#include "utils/Rectangle.hpp"
#include "utils/Vector2.hpp"
#include "zappy/gui/render/utils/Ray.hpp"

namespace zappy::gui::render {
class Camera;
}

namespace zappy::gui::game {
class GameState;
}

namespace zappy::gui::ui {

class PlayerInfo {
  public:
    PlayerInfo() = delete;

    static constexpr float kPickRadius = 0.5F;
    static constexpr float kPickHeight = 0.5F;

    [[nodiscard]] static bool intersectsPlayer(const render::Ray& ray, float playerX, float playerY, float radius);

    [[nodiscard]] static std::optional<std::uint32_t> pick(Vector2 mousePos, const render::Camera& camera,
                                                           const game::GameState& state);

    static void draw(const game::GameState& state, std::uint32_t playerId, Rectangle bounds);
};

}  // namespace zappy::gui::ui
