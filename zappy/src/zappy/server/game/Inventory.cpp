/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Inventory
*/

#include "Inventory.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "ResourceType.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"

namespace zappy::server::game {
std::uint32_t Inventory::resourceCount() const {
    return std::accumulate(_resources.begin(), _resources.end(), 0,
                           [](const auto& acc, const auto& resource) { return acc + resource.second; });
}

std::uint16_t Inventory::resourceCount(const ResourceType type) const {
    if (!isValidResourceType(type)) {
        throw exception::InvalidArgument{"invalid resource type"};
    }

    const auto it = _resources.find(type);

    if (it == _resources.end()) {
        return 0;
    }
    return it->second;
}

std::uint16_t Inventory::addResource(const ResourceType type, const std::uint16_t amount) {
    if (!isValidResourceType(type)) {
        throw exception::InvalidArgument{"invalid resource type"};
    }

    std::uint16_t& resourceAmount = _resources[type];

    if (constexpr std::uint16_t maxValue = std::numeric_limits<std::uint16_t>::max();
        amount > maxValue - resourceAmount) {
        resourceAmount = maxValue;
    } else {
        resourceAmount += amount;
    }
    return resourceAmount;
}

std::uint16_t Inventory::removeResource(const ResourceType type, const std::uint16_t amount) {
    if (!isValidResourceType(type)) {
        throw exception::InvalidArgument{"invalid resource type"};
    }

    std::uint16_t& resourceAmount = _resources[type];

    resourceAmount -= std::min(resourceAmount, amount);
    return resourceAmount;
}

void Inventory::clear() { _resources.clear(); }

std::string Inventory::string() const {
    std::stringstream stringStream;

    for (std::uint8_t i = 0; i < std::to_underlying(ResourceType::kCount); ++i) {
        if (i > 0) {
            stringStream << " ";
        }
        if (const auto it = _resources.find(ResourceType{i}); it != _resources.end()) {
            stringStream << it->second;
        } else {
            stringStream << 0;
        }
    }
    return stringStream.str();
}

[[nodiscard]] std::string Inventory::detailedString() const {
    std::stringstream stringStream;
    static const std::vector<std::string> resourcesName = {"food",     "linemate", "deraumere", "sibur",
                                                           "mendiane", "phiras",   "thystame"};

    for (std::uint8_t i = 0; i < std::to_underlying(ResourceType::kCount); ++i) {
        const auto it = _resources.find(ResourceType{i});
        const std::size_t count = (it != _resources.end()) ? it->second : 0;

        if (i > 0) {
            stringStream << ", ";
        }
        stringStream << resourcesName.at(i) << " " << count;
    }
    return stringStream.str();
}

bool Inventory::isValidResourceType(const ResourceType type) {
    return std::to_underlying(type) < std::to_underlying(ResourceType::kCount);
}
}  // namespace zappy::server::game
