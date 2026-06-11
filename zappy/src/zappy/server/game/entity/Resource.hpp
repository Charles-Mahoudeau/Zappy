/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Resource
*/

#pragma once

#include "zappy/server/game/IEntity.hpp"
#include "zappy/server/game/ResourceType.hpp"

namespace zappy::server::game::entity {
class Resource : public IEntity {
  public:
    explicit Resource(ResourceType type);
    ~Resource() override = default;

    Resource(const Resource&) = default;
    Resource& operator=(const Resource&) = default;

    Resource(Resource&&) = default;
    Resource& operator=(Resource&&) = default;

    /// @brief Update resource.
    void update() override;

    /// @brief Get resource type.
    /// @return Resource type.
    [[nodiscard]] ResourceType type() const;

  private:
    ResourceType _type;
};
}  // namespace zappy::server::game::entity
