/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** AssetStore
*/

#include "AssetStore.hpp"

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <map>
#include <memory>
#include <utility>

#include "AssetLoaderRegistry.hpp"
#include "objects/Model.hpp"
#include "objects/Texture.hpp"
#include "objects/particles/Particle.hpp"
#include "objects/particles/ParticleEmitter.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/render/utils/Color.hpp"
#include "zappy/gui/render/utils/ColorF.hpp"

namespace zappy::gui::render {

void AssetStore::load(const ProgressCallback& onProgress) {
    _playerModels.reserve(kMaxModels);

    for (const auto& [name, loadStep] : AssetLoaderRegistry::create(*this)) {
        loadStep();
        if (onProgress) {
            onProgress(name,
                       (static_cast<float>(_playerModels.size()) + static_cast<float>(_resourcesModels.size()) + 3.0F) /
                           (static_cast<float>(kMaxModels) +
                            static_cast<float>(std::to_underlying(game::ResourceType::Thystame)) + 1.0F + 3.0F));
        }
    }
}

void AssetStore::loadSkybox() { _skybox = Skybox{"assets/skybox/skybox.png", "assets/shaders", true}; }

void AssetStore::loadPlayerModel() {
    using enum Model::MaterialMapIndex;

    _playerModels.emplace_back(
        createModel("assets/models/slugs/burpy/source/burpy.glb", "assets/models/slugs/burpy/source/burpy.glb",
                    {{.mapIndex = ALBEDO, .path = "assets/models/slugs/burpy/textures/burpy.png"}}, true));
    _playerModels.emplace_back(createModel("assets/models/slugs/electra.glb", "assets/models/slugs/electra.glb", {}));
    _playerModels.emplace_back(createModel("assets/models/slugs/frozy.glb", "assets/models/slugs/frozy.glb", {}));
    _playerModels.emplace_back(createModel("assets/models/slugs/doc/doc.glb", "assets/models/slugs/doc/doc.glb",
                                           {{.mapIndex = ALBEDO, .path = "assets/models/slugs/doc/doc.png"}}, true));
    _playerModels.emplace_back(createModel("assets/models/slugs/bad1/bad1.glb", "assets/models/slugs/bad1/bad1.glb",
                                           {{.mapIndex = ALBEDO, .path = "assets/models/slugs/bad1/bad1.png"}}));
    _playerModels.emplace_back(createModel("assets/models/slugs/bad2/bad2.glb", "assets/models/slugs/bad2/bad2.glb",
                                           {{.mapIndex = ALBEDO, .path = "assets/models/slugs/bad2/bad2.png"}}));
}

void AssetStore::loadResourceModel(game::ResourceType type) {
    using enum Model::MaterialMapIndex;
    using enum game::ResourceType;

    switch (type) {
        case Food:
            _resourcesModels.emplace(Food, createModel("assets/models/resources/source/food.glb", {}));
            return;
        case Linemate:
            _resourcesModels.emplace(
                Linemate,
                createModel("assets/models/resources/source/linemate.glb",
                            {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/linemateBase.png"},
                             {.mapIndex = EMISSION, .path = "assets/models/resources/textures/linemateEmissive.png"},
                             {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                             {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
            return;
        case Deraumere:
            _resourcesModels.emplace(
                Deraumere,
                createModel("assets/models/resources/source/deraumere.glb",
                            {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/deraumereBase.png"},
                             {.mapIndex = EMISSION, .path = "assets/models/resources/textures/deraumereEmissive.png"},
                             {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                             {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
            return;
        case Sibur:
            _resourcesModels.emplace(
                Sibur,
                createModel("assets/models/resources/source/sibur.glb",
                            {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/siburBase.png"},
                             {.mapIndex = EMISSION, .path = "assets/models/resources/textures/siburEmissive.png"},
                             {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                             {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
            return;
        case Mendiane:
            _resourcesModels.emplace(
                Mendiane,
                createModel("assets/models/resources/source/mendiane.glb",
                            {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/mendianeBase.png"},
                             {.mapIndex = EMISSION, .path = "assets/models/resources/textures/mendianeEmissive.png"},
                             {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                             {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
            return;
        case Phiras:
            _resourcesModels.emplace(
                Phiras,
                createModel("assets/models/resources/source/phiras.glb",
                            {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/phirasBase.png"},
                             {.mapIndex = EMISSION, .path = "assets/models/resources/textures/phirasEmissive.png"},
                             {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                             {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
            return;
        case Thystame:
            _resourcesModels.emplace(
                Thystame,
                createModel("assets/models/resources/source/thystame.glb",
                            {{.mapIndex = ALBEDO, .path = "assets/models/resources/textures/thystameBase.png"},
                             {.mapIndex = EMISSION, .path = "assets/models/resources/textures/thystameEmissive.png"},
                             {.mapIndex = NORMAL, .path = "assets/models/resources/textures/normal1.png"},
                             {.mapIndex = OCCLUSION, .path = "assets/models/resources/textures/occlusionB.png"}}));
            return;
    }
}

void AssetStore::loadVFXs() {
    ParticleEmitter test1("assets/models/resources/textures/thystameBase.png");
    test1.setStatic(Vector3{}, Vector3{1.0F, 1.0F, 1.0F}, Vector3{0.0F, -0.002F, 0.0F}, 0.0F, 30.0F);
    test1.setInitParticles(Vec2D{1.0F, 1.0F}, 0.0F, ColorF(Color::kWHITE), 150.0F, 0.1F);
    test1.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.3F, .rotation = 0.5F, .tint = 0.3F}, 0.1F, 0.3F);
    test1.setIncrementParticles(Vec2D{-0.005F, -0.005F}, 1.0F, ColorF{0, 0, 0, -3.0F});
    test1.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.2F, .rotation = 0.4F, .tint = 0.1F});
    _vfxs.insert({"test1", std::move(test1)});

    ParticleEmitter test2("assets/models/resources/textures/thystameBase.png");
    test2.setStatic(Vector3{}, Vector3{1.0F, 1.0F, 1.0F}, Vector3{0.0F, -0.002F, 0.0F}, 30.0F, 30.0F);
    test2.setInitParticles(Vec2D{1.0F, 1.0F}, 0.0F, ColorF(Color::kWHITE), 150.0F, 0.1F);
    test2.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.3F, .rotation = 0.5F, .tint = 0.3F}, 0.1F, 0.3F);
    test2.setIncrementParticles(Vec2D{-0.005F, -0.005F}, 1.0F, ColorF{0, 0, 0, -3.0F});
    test2.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.2F, .rotation = 0.4F, .tint = 0.1F});
    _vfxs.insert({"test2", std::move(test2)});

    ParticleEmitter test3("assets/models/resources/textures/thystameBase.png");
    test3.setStatic(Vector3{}, Vector3{1.0F, 1.0F, 1.0F}, Vector3{0.0F, -0.002F, 0.0F}, 90.0F, 30.0F);
    test3.setInitParticles(Vec2D{1.0F, 1.0F}, 0.0F, ColorF(Color::kWHITE), 150.0F, 0.1F);
    test3.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.3F, .rotation = 0.5F, .tint = 0.3F}, 0.1F, 0.3F);
    test3.setIncrementParticles(Vec2D{-0.005F, -0.005F}, 1.0F, ColorF{0, 0, 0, -3.0F});
    test3.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.2F, .rotation = 0.4F, .tint = 0.1F});
    _vfxs.insert({"test3", std::move(test3)});

    ParticleEmitter test4("assets/models/resources/textures/thystameBase.png");
    test4.setStatic(Vector3{}, Vector3{1.0F, 1.0F, 1.0F}, Vector3{0.0F, -0.002F, 0.0F}, 180.0F, 30.0F);
    test4.setInitParticles(Vec2D{1.0F, 1.0F}, 0.0F, ColorF(Color::kWHITE), 150.0F, 0.1F);
    test4.setInitEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.3F, .rotation = 0.5F, .tint = 0.3F}, 0.1F, 0.3F);
    test4.setIncrementParticles(Vec2D{-0.005F, -0.005F}, 1.0F, ColorF{0, 0, 0, -3.0F});
    test4.setIncrementEnvelope(ParticleEmitter::ParticleEnvelope{.size = 0.2F, .rotation = 0.4F, .tint = 0.1F});
    _vfxs.insert({"test4", std::move(test4)});

    // Temporary test VFXs, will be removed later
}

void AssetStore::loadEggModel() {
    _eggModel = std::make_unique<Model>(
        createModel("assets/egg/source/EGG.glb",
                    {{.mapIndex = Model::MaterialMapIndex::ALBEDO, .path = "assets/egg/textures/egg_0.png"}}));
}

void AssetStore::loadIslandModel() {
    _islandModel = std::make_unique<Model>(createModel("assets/models/grid/island.glb", {}));
}

void AssetStore::loadBridgeModel() {
    _bridgeModel = std::make_unique<Model>(createModel("assets/models/grid/bridge.glb", {}));
}

const Skybox& AssetStore::skybox() const { return _skybox; }

Model& AssetStore::playerModel(std::size_t index) { return _playerModels.at(index); }

std::size_t AssetStore::playerModelCount() const { return _playerModels.size(); }

const std::map<game::ResourceType, Model>& AssetStore::resourceModels() const { return _resourcesModels; }

Model AssetStore::createModel(std::string_view path, std::initializer_list<TextureMap> textures, bool flipVertical) {
    Model model{path};
    for (const auto& [mapIndex, texturePath] : textures) {
        Texture texture{texturePath, flipVertical};
        model.setMeshTexture(0, mapIndex, texture);
        texture.release();
    }
    model.normalizeOnGround();

    return model;
}

Model AssetStore::createModel(std::string_view path, std::string_view animationPath,
                              std::initializer_list<TextureMap> textures, bool flipVertical) {
    Model model{path, animationPath};
    for (const auto& [mapIndex, texturePath] : textures) {
        Texture texture{texturePath, flipVertical};
        model.setMeshTexture(0, mapIndex, texture);
        texture.release();
    }
    model.useSkinningShader();
    model.normalizeOnGround();

    return model;
}

const std::map<std::string, ParticleEmitter>& AssetStore::vfxs() const { return _vfxs; }

std::map<std::string, ParticleEmitter>& AssetStore::vfxs() { return _vfxs; }

ParticleEmitter& AssetStore::emit(std::string_view path, Vector3 pos) {
    auto it = _vfxs.find(std::string{path});
    if (it == _vfxs.end()) throw std::runtime_error("VFX not found: " + std::string{path});
    ParticleEmitter& emitter = it->second;
    emitter.setOrigin(pos);
    emitter.emitRate();
    return emitter;
}
}  // namespace zappy::gui::render
