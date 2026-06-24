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

    static constexpr float kPickRadius = 0.9F;
    static constexpr float kPickHeight = 0.5F;

    [[nodiscard]] static bool intersectsPlayer(const render::Ray& ray, float playerX, float playerY, float radius);

    [[nodiscard]] static std::optional<std::uint32_t> pick(Vector2 mousePos, const render::Camera& camera,
                                                           const game::GameState& state);

    [[nodiscard]] static float height();

    static void draw(const game::GameState& state, std::uint32_t playerId, Rectangle bounds);

  private:
    static constexpr float kRowHeight = 24.0F;
    static constexpr float kTextMarginTop = 4.0F;
    static constexpr float kSectionGap = 8.0F;
    static constexpr int kPlacementRowCount = 2;
    static constexpr int kStateRowCount = 2;
    static constexpr int kInventoryRowCount = 7;

    [[nodiscard]] static float sectionHeight(int rowCount);
};

}  // namespace zappy::gui::ui
