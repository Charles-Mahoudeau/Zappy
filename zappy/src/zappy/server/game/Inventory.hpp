/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Inventory
*/

#pragma once

#include <compare>
#include <cstdint>
#include <string>
#include <unordered_map>

#include "ResourceType.hpp"

namespace zappy::server::game {
class Inventory {
  public:
    struct ResourcesInit {
        std::uint16_t food{0};
        std::uint16_t linemate{0};
        std::uint16_t deraumere{0};
        std::uint16_t sibur{0};
        std::uint16_t mendiane{0};
        std::uint16_t phiras{0};
        std::uint16_t thystame{0};
    };

    Inventory() = default;
    explicit Inventory(const ResourcesInit& resourcesInit);
    ~Inventory() = default;

    Inventory(const Inventory&) = default;
    Inventory& operator=(const Inventory&) = default;

    Inventory(Inventory&&) = default;
    Inventory& operator=(Inventory&&) = default;

    /// @brief Get the count of all resources in the inventory.
    /// @return The count of all resources in the inventory.
    [[nodiscard]] std::uint32_t resourceCount() const;

    /// @brief Get the count of a resource in the inventory.
    /// @param type The type of resource to get the count of.
    /// @return The count of the resource in the inventory.
    [[nodiscard]] std::uint16_t resourceCount(ResourceType type) const;

    /// @brief Add a resource to the inventory.
    /// @param type The type of resource to add.
    /// @param amount The amount of the resource to add.
    /// @return The new amount of the resource in the inventory.
    std::uint16_t addResource(ResourceType type, std::uint16_t amount = 1);

    /// @brief Remove a resource from the inventory.
    /// @param type The type of resource to remove.
    /// @param amount The amount of the resource to remove.
    /// @return The new amount of the resource in the inventory.
    std::uint16_t removeResource(ResourceType type, std::uint16_t amount = 1);

    /// @brief Clear the inventory.
    void clear();

    /// @brief Check if the inventory can afford another inventory.
    /// @param other The inventory to check.
    /// @return True if the inventory can afford the other inventory, false otherwise.
    [[nodiscard]] bool canAfford(const Inventory& other) const;

    /// @brief Get the string representation of the inventory.
    /// @return The string representation of the inventory.
    [[nodiscard]] std::string string() const;

    [[nodiscard]] std::string detailedString() const;

    Inventory& operator+=(const Inventory& other);
    Inventory& operator-=(const Inventory& other);

    [[nodiscard]] std::partial_ordering operator<=>(const Inventory& other) const;

  private:
    /// @brief Check if a resource type is valid.
    /// @param type The type of resource to check.
    /// @return True if the resource type is valid, false otherwise.
    [[nodiscard]] static bool isValidResourceType(ResourceType type);

    std::unordered_map<ResourceType, std::uint16_t> _resources;
};
}  // namespace zappy::server::game
