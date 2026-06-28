/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** IEntity
*/

#pragma once

#include <concepts>
#include <cstdint>
#include <string_view>

#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
/// @brief Interface for entities in the game.
class IEntity {
  public:
    IEntity() = default;
    virtual ~IEntity() = default;

    IEntity(const IEntity&) = default;
    IEntity& operator=(const IEntity&) = default;

    IEntity(IEntity&&) = default;
    IEntity& operator=(IEntity&&) = default;

    /// @brief Get the entity's id.
    /// @return The entity's id.
    [[nodiscard]] virtual std::uint64_t id() const = 0;

    /// @brief Set the entity's id.
    /// @param id The entity's id.
    virtual void setId(std::uint64_t id) = 0;

    /// @brief Get the entity's position.
    /// @return The entity's position.
    [[nodiscard]] virtual math::Vector2u position() const = 0;

    /// @brief Set the entity's position.
    /// @param position The entity's position.
    virtual void setPosition(math::Vector2u position) = 0;

    /// @brief Get the entity's team name.
    /// @return The entity's team name.
    [[nodiscard]] virtual std::string_view teamName() const = 0;
};

template <typename T>
concept IsEntity = std::derived_from<T, IEntity>;
}  // namespace zappy::server::game
