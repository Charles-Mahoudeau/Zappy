/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** AEntity
*/

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <string_view>

#include "IEntity.hpp"
#include "IEventEmitter.hpp"
#include "IGrid.hpp"
#include "zappy/server/Timer.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game {
class AEntity : public IEntity {
  public:
    AEntity(Timer& timer, IGrid& grid, IEventEmitter& eventEmitter, std::string teamName);
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

    /// @brief Get the entity's position.
    /// @return The entity's position.
    [[nodiscard]] math::Vector2u position() const override;

    /// @brief Set the entity's position.
    /// @param position The entity's position.
    void setPosition(math::Vector2u position) override;

    /// @brief Get the entity's team name.
    /// @return The entity's team name.
    [[nodiscard]] std::string_view teamName() const override;

  protected:
    [[nodiscard]] Timer& timer() const;
    [[nodiscard]] IEventEmitter& eventEmitter() const;

  private:
    std::uint64_t _id{};
    std::reference_wrapper<Timer> _timer;
    std::reference_wrapper<IGrid> _grid;
    std::reference_wrapper<IEventEmitter> _eventEmitter;
    std::string _teamName;
};
}  // namespace zappy::server::game
