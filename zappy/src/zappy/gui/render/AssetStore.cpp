/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** AssetStore
*/

#include "AssetStore.hpp"

#include <cstddef>
#include <initializer_list>
#include <map>
#include <string>

#include "AssetLoaderRegistry.hpp"
#include "objects/Model.hpp"
#include "objects/Texture.hpp"
#include "zappy/gui/game/GameState.hpp"

namespace zappy::gui::render {

void AssetStore::load(const ProgressCallback& onProgress) {
    _playerModels.reserve(kMaxModels);

    for (const auto& [name, loadStep] : AssetLoaderRegistry::create(*this)) {
        loadStep();
        if (onProgress) {
            onProgress();
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

const Skybox& AssetStore::skybox() const { return _skybox; }

Model& AssetStore::playerModel(std::size_t index) { return _playerModels.at(index); }

std::size_t AssetStore::playerModelCount() const { return _playerModels.size(); }

const std::map<game::ResourceType, Model>& AssetStore::resourceModels() const { return _resourcesModels; }

Model AssetStore::createModel(std::string_view path, std::initializer_list<TextureMap> textures, bool flipVertical) {
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

Model AssetStore::createModel(std::string_view path, std::string_view animationPath,
                              std::initializer_list<TextureMap> textures, bool flipVertical) {
    Model model{path, animationPath};
    for (const auto& [mapIndex, texturePath] : textures) {
        const auto texturePathStr = std::string{texturePath};
        Texture texture{texturePathStr.c_str(), flipVertical};
        model.setMeshTexture(0, mapIndex, texture);
        texture.release();
    }
    model.useSkinningShader();
    model.normalizeOnGround();

    return model;
}

}  // namespace zappy::gui::render
