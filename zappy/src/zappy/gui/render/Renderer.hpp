/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Renderer
*/

#pragma once

#include <cstdint>
#include <initializer_list>
#include <map>
#include <string_view>
#include <vector>

#include "Camera.hpp"
#include "objects/Model.hpp"
#include "objects/Skybox.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::render {
class Renderer {
  public:
    Renderer() = default;
    ~Renderer() = default;
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    Renderer(Renderer&& other) noexcept = default;
    Renderer& operator=(Renderer&& other) noexcept = default;

    void init();
    void update(Camera& camera, game::GameState& state);

  private:
    struct TextureMap {
        Model::MaterialMapIndex mapIndex;
        std::string_view path;
    };

    static Model createModel(std::string_view path, std::initializer_list<TextureMap> textures,
                             bool flipVertical = false);
    static Model createModel(std::string_view path, std::string_view animationPath,
                             std::initializer_list<TextureMap> textures, bool flipVertical = false);

    static void drawGrid(const game::GameState& state);

    void drawResources(const game::GameState& state);
    void drawTileResources(const game::Resources& tile, const Vector3& position);
    static void drawResourceStack(const Model& model, const Vector3& position, std::uint32_t count);
    static std::uint32_t resourceCount(const game::Resources& tile, game::ResourceType type);

    void drawPlayers(const game::GameState& state);
    static float calculAngle(game::Orientation orientation);

    static constexpr int kMaxModels = 10;
    static constexpr float kScale = 0.5F;
    std::vector<Model> _playerModels;
    std::map<game::ResourceType, Model> _resourcesModels;
    Skybox _skybox;
    // Grid _grid; TODO: Implement a grid class to render a grid in the scene
};
}  // namespace zappy::gui::render
