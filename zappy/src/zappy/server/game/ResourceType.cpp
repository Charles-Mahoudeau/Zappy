/*
** EPITECH PROJECT, 2026
** ResourceType
** File description:
** ResourceType code
*/

#include "zappy/server/game/ResourceType.hpp"

#include <optional>
#include <string_view>
#include <unordered_map>
namespace zappy::server::game {

std::unordered_map<std::string_view, game::ResourceType> ResourceHelper::map() {
    return {{{"food", kFood},
             {"linemate", kLinemate},
             {"deraumere", kDeraumere},
             {"sibur", kSibur},
             {"mendiane", kMendiane},
             {"phiras", kPhiras},
             {"thystame", kThystame}}};
}

std::optional<ResourceType> ResourceHelper::strToRessource(std::string_view resource) {
    auto map = ResourceHelper::map();

    if (auto iter = map.find(resource); iter != map.end()) {
        return iter->second;
    }
    return std::nullopt;
}

std::string_view ResourceHelper::ressourceToString(ResourceType resource) {
    auto map = ResourceHelper::map();

    for (const auto& [name, type] : map) {
        if (resource == type) {
            return name;
        }
    }
    return {};
}

}  // namespace zappy::server::game
