/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** AssetStore
*/

#pragma once

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "objects/Model.hpp"
#include "objects/Skybox.hpp"
#include "objects/particles/Particle.hpp"
#include "objects/particles/ParticleEmitter.hpp"
#include "zappy/gui/game/GameState.hpp"

namespace zappy::gui::render {

class AssetStore {
  public:
    using ProgressCallback = std::function<void(std::string_view name, float progress)>;

    AssetStore() = default;
    ~AssetStore() = default;

    AssetStore(const AssetStore&) = delete;
    AssetStore& operator=(const AssetStore&) = delete;

    AssetStore(AssetStore&& other) noexcept = default;
    AssetStore& operator=(AssetStore&& other) noexcept = default;

    void load(const ProgressCallback& onProgress = nullptr);

    void loadSkybox();
    void loadPlayerModel();
    void loadResourceModel(game::ResourceType type);
    void loadVFXs();
    void loadEggModel();
    void loadIslandModel();
    void loadBridgeModel();

    [[nodiscard]] const Skybox& skybox() const;
    [[nodiscard]] Model& playerModel(std::size_t index);
    [[nodiscard]] std::size_t playerModelCount() const;
    [[nodiscard]] const std::map<game::ResourceType, Model>& resourceModels() const;
    [[nodiscard]] const std::map<std::string_view, ParticleEmitter>& vfxs() const;
    [[nodiscard]] std::map<std::string_view, ParticleEmitter>& vfxs();
    [[nodiscard]] const Model& eggModel() const { return *_eggModel; }
    [[nodiscard]] const Model& islandModel() const { return *_islandModel; }
    [[nodiscard]] const Model& bridgeModel() const { return *_bridgeModel; }

  private:
    struct TextureMap {
        Model::MaterialMapIndex mapIndex;
        std::string_view path;
    };

    static Model createModel(std::string_view path, std::initializer_list<TextureMap> textures,
                             bool flipVertical = false);
    static Model createModel(std::string_view path, std::string_view animationPath,
                             std::initializer_list<TextureMap> textures, bool flipVertical = false);
    static constexpr int kMaxModels = 6;

    static ParticleEmitter emit(std::string_view path, Vector3 pos);

    Skybox _skybox;
    std::vector<Model> _playerModels;
    std::map<game::ResourceType, Model> _resourcesModels;
    std::map<std::string_view, ParticleEmitter> _vfxs;
    std::unique_ptr<Model> _eggModel;
    std::unique_ptr<Model> _islandModel;
    std::unique_ptr<Model> _bridgeModel;
};

}  // namespace zappy::gui::render
