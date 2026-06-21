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
    using Map = Model::MaterialMapIndex;

    _playerModels.reserve(kMaxModels);
    _playerModels.emplace_back(createModel("assets/models/burpy/source/burpy.glb",
                                           {{Map::ALBEDO, "assets/models/burpy/textures/burpy.png"}}, true));

    _resourcesModels.emplace(game::RessourceType::Food, createModel("assets/models/resources/source/food.glb", {}));
    _resourcesModels.emplace(game::RessourceType::Linemate,
                             createModel("assets/models/resources/source/linemate.glb",
                                         {{Map::ALBEDO, "assets/models/resources/textures/linemateBase.png"},
                                          {Map::EMISSION, "assets/models/resources/textures/linemateEmissive.png"},
                                          {Map::NORMAL, "assets/models/resources/textures/normal1.png"},
                                          {Map::OCCLUSION, "assets/models/resources/textures/occlusionB.png"}}));
    _resourcesModels.emplace(game::RessourceType::Deraumere,
                             createModel("assets/models/resources/source/deraumere.glb",
                                         {{Map::ALBEDO, "assets/models/resources/textures/deraumereBase.png"},
                                          {Map::EMISSION, "assets/models/resources/textures/deraumereEmissive.png"},
                                          {Map::NORMAL, "assets/models/resources/textures/normal1.png"},
                                          {Map::OCCLUSION, "assets/models/resources/textures/occlusionB.png"}}));
    _resourcesModels.emplace(game::RessourceType::Sibur,
                             createModel("assets/models/resources/source/sibur.glb",
                                         {{Map::ALBEDO, "assets/models/resources/textures/siburBase.png"},
                                          {Map::EMISSION, "assets/models/resources/textures/siburEmissive.png"},
                                          {Map::NORMAL, "assets/models/resources/textures/normal1.png"},
                                          {Map::OCCLUSION, "assets/models/resources/textures/occlusionB.png"}}));
    _resourcesModels.emplace(game::RessourceType::Mendiane,
                             createModel("assets/models/resources/source/mendiane.glb",
                                         {{Map::ALBEDO, "assets/models/resources/textures/mendianeBase.png"},
                                          {Map::EMISSION, "assets/models/resources/textures/mendianeEmissive.png"},
                                          {Map::NORMAL, "assets/models/resources/textures/normal1.png"},
                                          {Map::OCCLUSION, "assets/models/resources/textures/occlusionB.png"}}));
    _resourcesModels.emplace(game::RessourceType::Phiras,
                             createModel("assets/models/resources/source/phiras.glb",
                                         {{Map::ALBEDO, "assets/models/resources/textures/phirasBase.png"},
                                          {Map::EMISSION, "assets/models/resources/textures/phirasEmissive.png"},
                                          {Map::NORMAL, "assets/models/resources/textures/normal1.png"},
                                          {Map::OCCLUSION, "assets/models/resources/textures/occlusionB.png"}}));
    _resourcesModels.emplace(game::RessourceType::Thystame,
                             createModel("assets/models/resources/source/thystame.glb",
                                         {{Map::ALBEDO, "assets/models/resources/textures/thystameBase.png"},
                                          {Map::EMISSION, "assets/models/resources/textures/thystameEmissive.png"},
                                          {Map::NORMAL, "assets/models/resources/textures/normal1.png"},
                                          {Map::OCCLUSION, "assets/models/resources/textures/occlusionB.png"}}));
}

void Renderer::update(Camera& camera, game::GameState& state) {
    camera.Update();

    const auto& teams = state.teams();
    display::Window::BeginMode3D(camera);
    drawGrid(state);  // TODO: Implement a grid class to render a grid in the scene
    for (const auto& entry : state.players()) {
        const auto& player = entry.second;
        const auto teamIt = std::ranges::find(teams, player.team);
        const auto teamIndex = static_cast<std::size_t>(std::distance(teams.begin(), teamIt));
        const auto& model = _playerModels.at(teamIndex % _playerModels.size());
        const float scale = kScale * static_cast<float>(player.level);
        Vector3 position(static_cast<float>(player.x), 0.0F, static_cast<float>(player.y));
        model.drawEx(position, Vector3{0.0F, 1.0F, 0.0F}, calculAngle(player.orientation), scale, Color::WHITE);
    }

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
        const float offsetX = static_cast<float>(i % 2) * 0.5F - 0.25F;
        const float offsetZ = static_cast<float>(i / 2) * 0.5F - 0.25F;
        const Vector3 resourcePosition(position.x() + offsetX, position.y(), position.z() + offsetZ);
        model.draw(resourcePosition, kScale, Color::WHITE);
    }
}

std::uint32_t Renderer::resourceCount(const game::Resources& tile, game::RessourceType type) {
    switch (type) {
        case game::RessourceType::Food:
            return tile.food;
        case game::RessourceType::Linemate:
            return tile.linemate;
        case game::RessourceType::Deraumere:
            return tile.deraumere;
        case game::RessourceType::Sibur:
            return tile.sibur;
        case game::RessourceType::Mendiane:
            return tile.mendiane;
        case game::RessourceType::Phiras:
            return tile.phiras;
        case game::RessourceType::Thystame:
            return tile.thystame;
    }
    return 0;
}

Model Renderer::createModel(std::string_view path, std::initializer_list<TextureMap> textures, bool flipVertical) {
    Model model{path};
    for (const auto& [mapIndex, texturePath] : textures) {
        const std::string texturePathStr = std::string{texturePath};
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
    const ::Color color{60, 60, 60, 255};
    const auto fWidth = static_cast<float>(width);
    const auto fHeight = static_cast<float>(height);

    for (int x = 0; x <= width; ++x) {
        const auto fx = static_cast<float>(x) - kOffset;
        DrawLine3D(::Vector3{fx, 0.0F, -kOffset}, ::Vector3{fx, 0.0F, fHeight - kOffset}, color);
    }
    for (int z = 0; z <= height; ++z) {
        const auto fz = static_cast<float>(z) - kOffset;
        DrawLine3D(::Vector3{-kOffset, 0.0F, fz}, ::Vector3{fWidth - kOffset, 0.0F, fz}, color);
    }
}

float Renderer::calculAngle(game::Orientation orientation) {
    switch (orientation) {
        case game::Orientation::South:
            return 0.0F;
        case game::Orientation::East:
            return 90.0F;
        case game::Orientation::North:
            return 180.0F;
        case game::Orientation::West:
            return 270.0F;
        default:
            return 0.0F;
    }
}

}  // namespace zappy::gui::render
