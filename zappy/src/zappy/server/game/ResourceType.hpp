/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ResourceType
*/

#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace zappy::server::game {

enum class ResourceType : std::uint8_t {
    kFood,
    kLinemate,
    kDeraumere,
    kSibur,
    kMendiane,
    kPhiras,
    kThystame,
    kCount,
};

class ResourceHelper {
    using enum ResourceType;

  public:
    ResourceHelper() = delete;
    ResourceHelper(const ResourceHelper&) = delete;
    ResourceHelper(ResourceHelper&&) = delete;
    ResourceHelper& operator=(const ResourceHelper&) = delete;
    ResourceHelper& operator=(ResourceHelper&&) = delete;
    ~ResourceHelper() = delete;

    static std::optional<ResourceType> strToRessource(std::string_view resource) {
        auto map = ResourceHelper::map();

        if (auto iter = map.find(resource); iter != map.end()) {
            return iter->second;
        }
        return std::nullopt;
    }

    static std::string_view ressourceToString(ResourceType resource) {
        auto map = ResourceHelper::map();

        for (const auto& [name, type] : map) {
            if (resource == type) {
                return name;
            }
        }
        return {};
    }

  private:
    static std::unordered_map<std::string_view, game::ResourceType> map();
};

}  // namespace zappy::server::game
