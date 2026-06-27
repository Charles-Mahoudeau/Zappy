/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Inventory
*/

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include "ResourceType.hpp"

namespace zappy::server::game {
class Inventory {
  public:
    Inventory() = default;
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

    /// @brief Get the string representation of the inventory.
    /// @return The string representation of the inventory.
    [[nodiscard]] std::string string() const;

    [[nodiscard]] std::string detailedString() const;

  private:
    /// @brief Check if a resource type is valid.
    /// @param type The type of resource to check.
    /// @return True if the resource type is valid, false otherwise.
    [[nodiscard]] static bool isValidResourceType(ResourceType type);

    std::unordered_map<ResourceType, std::uint16_t> _resources;
};
}  // namespace zappy::server::game
