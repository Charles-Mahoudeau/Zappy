/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Inventory
*/

#include "Inventory.hpp"

#include <algorithm>
#include <cstdint>
#include <numeric>

#include "ResourceType.hpp"

namespace zappy::server::game {
std::uint32_t Inventory::resourceCount() const {
    return std::accumulate(_resources.begin(), _resources.end(), 0,
                           [](const auto& acc, const auto& resource) { return acc + resource.second; });
}

std::uint16_t Inventory::resourceCount(const ResourceType type) const {
    const auto it = _resources.find(type);

    if (it == _resources.end()) {
        return 0;
    }
    return it->second;
}

std::uint16_t Inventory::addResource(const ResourceType type, const std::uint16_t amount) {
    std::uint16_t& resourceAmount = _resources[type];

    resourceAmount += amount;
    return resourceAmount;
}

std::uint16_t Inventory::removeResource(const ResourceType type, std::uint16_t amount) {
    std::uint16_t& resourceAmount = _resources[type];

    resourceAmount -= std::min(resourceAmount, amount);
    return resourceAmount;
}

void Inventory::clear() { _resources.clear(); }
}  // namespace zappy::server::game
