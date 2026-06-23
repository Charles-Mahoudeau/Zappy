/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** TileInfo
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <utility>

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

class TileInfo {
  public:
    TileInfo() = delete;

    [[nodiscard]] static std::optional<std::pair<std::uint32_t, std::uint32_t>> intersectGroundPlane(
        const render::Ray& ray, std::size_t mapWidth, std::size_t mapHeight);

    [[nodiscard]] static std::optional<std::pair<std::uint32_t, std::uint32_t>> pick(Vector2 mousePos,
                                                                                     const render::Camera& camera,
                                                                                     const game::GameState& state);

    static void draw(const game::GameState& state, std::uint32_t x, std::uint32_t y, Rectangle bounds);
};

}  // namespace zappy::gui::ui
