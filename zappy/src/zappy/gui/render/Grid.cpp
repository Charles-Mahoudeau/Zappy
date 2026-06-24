/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Grid
*/

#include "Grid.hpp"

#include <cstddef>

#include "AssetStore.hpp"
#include "objects/Model.hpp"
#include "zappy/gui/render/utils/Color.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::render {

void Grid::resize(std::size_t width, std::size_t height) {
    _width = width;
    _height = height;
}

Vector3 Grid::tileToWorld(float tileX, float tileY, float elevation) {
    return {tileX * kSpacing, elevation, tileY * kSpacing};
}

void Grid::draw(const AssetStore& assets) const {
    if (_width == 0 || _height == 0) {
        return;
    }
    drawTiles(assets);
    drawBridges(assets);
}

void Grid::drawTiles(const AssetStore& assets) const {
    for (std::size_t y = 0; y < _height; ++y) {
        for (std::size_t x = 0; x < _width; ++x) {
            const Vector3 position = tileToWorld(static_cast<float>(x), static_cast<float>(y), kYOffset);
            assets.islandModel().draw(position, kTileScale, Color::kWHITE);
        }
    }
}

void Grid::drawBridges(const AssetStore& assets) const {
    constexpr float kHalfTile = 0.5F;
    const Model& bridge = assets.bridgeModel();

    for (std::size_t y = 0; y < _height; ++y) {
        for (std::size_t x = 0; x + 1 < _width; ++x) {
            const Vector3 position = tileToWorld(static_cast<float>(x) + kHalfTile, static_cast<float>(y));
            bridge.draw(position, kBridgeScale, Color::kWHITE);
        }
    }

    for (std::size_t y = 0; y + 1 < _height; ++y) {
        for (std::size_t x = 0; x < _width; ++x) {
            const Vector3 position = tileToWorld(static_cast<float>(x), static_cast<float>(y) + kHalfTile);
            bridge.drawEx(position, Vector3(0.0F, 1.0F, 0.0F), 90.0F, kBridgeScale, Color::kWHITE);
        }
    }
}

}  // namespace zappy::gui::render
