/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** AssetLoaderRegistry
*/

#include "AssetLoaderRegistry.hpp"

#include "AssetStore.hpp"
#include "zappy/gui/game/GameState.hpp"

namespace zappy::gui::render {

AssetLoaderList AssetLoaderRegistry::create(AssetStore& assets) {
    using enum game::ResourceType;

    return {
        {"skybox", [&assets]() { assets.loadSkybox(); }},
        {"player model", [&assets]() { assets.loadPlayerModel(); }},
        {"food", [&assets]() { assets.loadResourceModel(Food); }},
        {"linemate", [&assets]() { assets.loadResourceModel(Linemate); }},
        {"deraumere", [&assets]() { assets.loadResourceModel(Deraumere); }},
        {"sibur", [&assets]() { assets.loadResourceModel(Sibur); }},
        {"mendiane", [&assets]() { assets.loadResourceModel(Mendiane); }},
        {"phiras", [&assets]() { assets.loadResourceModel(Phiras); }},
        {"thystame", [&assets]() { assets.loadResourceModel(Thystame); }},
    };
}

}  // namespace zappy::gui::render
