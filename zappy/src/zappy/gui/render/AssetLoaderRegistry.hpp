/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** AssetLoaderRegistry
*/

#pragma once

#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace zappy::gui::render {

class AssetStore;

using AssetLoader = std::function<void()>;
using AssetLoaderList = std::vector<std::pair<std::string, AssetLoader>>;

class AssetLoaderRegistry {
  public:
    AssetLoaderRegistry() = delete;

    [[nodiscard]] static AssetLoaderList create(AssetStore& assets);
};

}  // namespace zappy::gui::render
