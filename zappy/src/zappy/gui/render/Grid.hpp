/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Grid
*/

#pragma once

#include <cstddef>

#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::render {

class AssetStore;
class Grid {
  public:
    Grid() = default;
    ~Grid() = default;
    Grid(const Grid&) = default;
    Grid& operator=(const Grid&) = default;
    Grid(Grid&&) noexcept = default;
    Grid& operator=(Grid&&) noexcept = default;

    void resize(std::size_t width, std::size_t height);

    [[nodiscard]] std::size_t width() const { return _width; }
    [[nodiscard]] std::size_t height() const { return _height; }
    [[nodiscard]] float worldWidth() const { return static_cast<float>(_width) * kSpacing; }
    [[nodiscard]] float worldHeight() const { return static_cast<float>(_height) * kSpacing; }
    [[nodiscard]] Vector3 tileToWorld(float tileX, float tileY, float elevation = 0.0F) const;

    void draw(const AssetStore& assets) const;

    static constexpr float kSpacing = 2.0F;

  private:
    void drawTiles(const AssetStore& assets) const;
    void drawBridges(const AssetStore& assets) const;

    static constexpr float kTileScale = 1.5F;
    static constexpr float kBridgeScale = 1.0F;

    static constexpr float kYOffset = -0.4F;

    std::size_t _width{0};
    std::size_t _height{0};
};

}  // namespace zappy::gui::render
