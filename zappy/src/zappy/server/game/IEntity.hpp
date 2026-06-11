/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** IEntity
*/

#pragma once

#include <concepts>

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

    /// @brief Update the entity.
    /// @note Call this once for each game tick.
    virtual void update() = 0;
};

template <typename T>
concept IsEntity = std::derived_from<T, IEntity>;
}  // namespace zappy::server::game
