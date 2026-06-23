/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** AEntity
*/

#pragma once

#include <cstdint>

#include "IEntity.hpp"

namespace zappy::server::game {
class AEntity : public IEntity {
  public:
    AEntity() = default;
    ~AEntity() override = default;

    AEntity(const AEntity&) = default;
    AEntity& operator=(const AEntity&) = default;

    AEntity(AEntity&&) = default;
    AEntity& operator=(AEntity&&) = default;

    /// @brief Get the entity's id.
    /// @return The entity's id.
    [[nodiscard]] std::uint64_t id() const override;

    /// @brief Set the entity's id.
    /// @param id The entity's id.
    void setId(std::uint64_t id) override;

  private:
    std::uint64_t _id{};
};
}  // namespace zappy::server::game
