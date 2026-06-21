/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Renderer
*/

#include "Renderer.hpp"

#include <raylib.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <string>
#include <string_view>

#include "Camera.hpp"
#include "objects/Model.hpp"
#include "objects/Texture.hpp"
#include "zappy/gui/display/Window.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/utils/Color.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::render {

void Renderer::init() {
    using enum Model::MaterialMapIndex;
    using enum game::ResourceType;

    _playerModels.reserve(kMaxModels);
    _playerModels.emplace_back(createModel("assets/models/burpy/source/burpy.glb",
                                           {{.mapIndex = ALBEDO, .path = "assets/models/burpy/textures/burpy.png"}},
                                           true));

    _resourcesModels.emplace(Food, createModel("assets/models/resources/source/food.glb", {}));
    _resourcesModels.emplace(
        Linemate, createModel("assets/models/resources/source/linemate.glb",
                              {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/linemateBase.png"},
                               {.mapIndex = EMISSION, .path = "assets/models/resources/textures/linemateEmissive.png"},
                               {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                               {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
    _resourcesModels.emplace(
        Deraumere,
        createModel("assets/models/resources/source/deraumere.glb",
                    {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/deraumereBase.png"},
                     {.mapIndex = EMISSION, .path = "assets/models/resources/textures/deraumereEmissive.png"},
                     {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                     {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
    _resourcesModels.emplace(
        Sibur, createModel("assets/models/resources/source/sibur.glb",
                           {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/siburBase.png"},
                            {.mapIndex = EMISSION, .path = "assets/models/resources/textures/siburEmissive.png"},
                            {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                            {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
    _resourcesModels.emplace(
        Mendiane, createModel("assets/models/resources/source/mendiane.glb",
                              {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/mendianeBase.png"},
                               {.mapIndex = EMISSION, .path = "assets/models/resources/textures/mendianeEmissive.png"},
                               {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                               {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
    _resourcesModels.emplace(
        Phiras, createModel("assets/models/resources/source/phiras.glb",
                            {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/phirasBase.png"},
                             {.mapIndex = EMISSION, .path = "assets/models/resources/textures/phirasEmissive.png"},
                             {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                             {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
    _resourcesModels.emplace(
        Thystame, createModel("assets/models/resources/source/thystame.glb",
                              {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/thystameBase.png"},
                               {.mapIndex = EMISSION, .path = "assets/models/resources/textures/thystameEmissive.png"},
                               {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                               {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
}

void Renderer::update(Camera& camera, game::GameState& state) {
    camera.Update();
    display::Window::BeginMode3D(camera);
    drawGrid(state);  // TODO: Implement a grid class to render a grid in the scene
    drawPlayers(state);
    drawResources(state);
    display::Window::EndMode3D();
}

void Renderer::drawResources(const game::GameState& state) {
    for (std::size_t y = 0; y < state.height(); ++y) {
        for (std::size_t x = 0; x < state.width(); ++x) {
            const Vector3 position(static_cast<float>(x), 0.0F, static_cast<float>(y));
            drawTileResources(state.tile(x, y), position);
        }
    }
}

void Renderer::drawTileResources(const game::Resources& tile, const Vector3& position) {
    for (const auto& [resourceType, model] : _resourcesModels) {
        drawResourceStack(model, position, resourceCount(tile, resourceType));
    }
}

void Renderer::drawResourceStack(const Model& model, const Vector3& position, std::uint32_t count) {
    for (std::uint32_t i = 0; i < count; ++i) {
        const float offsetX = (static_cast<float>(i % 2) * 0.5F) - 0.25F;
        const float offsetZ = ((static_cast<float>(i) / 2.0F) * 0.5F) - 0.25F;
        const Vector3 resourcePosition(position.x() + offsetX, position.y(), position.z() + offsetZ);
        model.draw(resourcePosition, kScale, Color::kWHITE);
    }
}

std::uint32_t Renderer::resourceCount(const game::Resources& tile, game::ResourceType type) {
    using enum game::ResourceType;
    switch (type) {
        case Food:
            return tile.food;
        case Linemate:
            return tile.linemate;
        case Deraumere:
            return tile.deraumere;
        case Sibur:
            return tile.sibur;
        case Mendiane:
            return tile.mendiane;
        case Phiras:
            return tile.phiras;
        case Thystame:
            return tile.thystame;
    }
    return 0;
}

Model Renderer::createModel(std::string_view path, std::initializer_list<TextureMap> textures, bool flipVertical) {
    Model model{path};
    for (const auto& [mapIndex, texturePath] : textures) {
        const auto texturePathStr = std::string{texturePath};
        Texture texture{texturePathStr.c_str(), flipVertical};
        model.setMeshTexture(0, mapIndex, texture);
        texture.release();
    }
    model.normalizeOnGround();

    return model;
}

// TODO: real grid
void Renderer::drawGrid(const game::GameState& state) {
    const auto width = static_cast<int>(state.width());
    const auto height = static_cast<int>(state.height());
    if (width <= 0 || height <= 0) {
        return;
    }
    constexpr float kOffset = 0.5F;
    const Color color(60, 60, 60, 255);
    const auto fWidth = static_cast<float>(width);
    const auto fHeight = static_cast<float>(height);

    for (int x = 0; x <= width; ++x) {
        const auto fx = static_cast<float>(x) - kOffset;
        DrawLine3D(Vector3(fx, 0.0F, -kOffset), Vector3(fx, 0.0F, fHeight - kOffset), color);
    }
    for (int z = 0; z <= height; ++z) {
        const auto fz = static_cast<float>(z) - kOffset;
        DrawLine3D(Vector3(-kOffset, 0.0F, fz), Vector3(fWidth - kOffset, 0.0F, fz), color);
    }
}

void Renderer::drawPlayers(const game::GameState& state) {
    const auto& teams = state.teams();
    for (const auto& [playerId, player] : state.players()) {
        const auto teamIt = std::ranges::find(teams, player.team);
        const auto teamIndex = static_cast<std::size_t>(std::distance(teams.begin(), teamIt));
        const auto& model = _playerModels.at(teamIndex % _playerModels.size());
        const float scale = kScale * static_cast<float>(player.level);
        Vector3 position(static_cast<float>(player.x), 0.0F, static_cast<float>(player.y));
        model.drawEx(position, Vector3(0.0F, 1.0F, 0.0F), calculAngle(player.orientation), scale, Color::kWHITE);
    }
}

float Renderer::calculAngle(game::Orientation orientation) {
    using enum game::Orientation;
    switch (orientation) {
        case South:
            return 0.0F;
        case East:
            return 90.0F;
        case North:
            return 180.0F;
        case West:
            return 270.0F;
        default:
            return 0.0F;
    }
}

}  // namespace zappy::gui::render
