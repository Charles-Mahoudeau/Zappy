/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Player
*/

#pragma once

#include <cstdint>
#include <expected>
#include <string>

#include "zappy/server/game/AEntity.hpp"
#include "zappy/server/game/Inventory.hpp"
#include "zappy/server/game/ResourceType.hpp"
#include "zappy/shared/math/Direction.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::game::entity {
class Player : public AEntity {
  public:
    static constexpr std::uint8_t kMaxLevel{8};
    static constexpr std::uint8_t kTimeUnitsPerFood{126};
    static constexpr std::uint8_t kInitialFoodAmount{10};

    Player(Timer& timer, IGrid& grid, IEventEmitter& eventEmitter, std::string teamName);
    ~Player() override;

    /// @brief Check if the player is alive.
    /// @return True if the player is alive, false otherwise.
    [[nodiscard]] bool alive() const;

    /// @brief Kill the player.
    void kill();

    /// @brief Get the player's level.
    /// @return The player's level.
    [[nodiscard]] std::uint8_t level() const;

    /// @brief Increase the player's level.
    /// @return The new level of the player.
    std::expected<std::uint8_t, std::string> levelUp();

    /// @brief Set the player's position.
    /// @param position The new position of the player.
    void setPosition(math::Vector2u position) override;

    /// @brief Get the player's direction.
    /// @return The player's direction.
    [[nodiscard]] math::Direction orientation() const;

    /// @brief Turn the player to the left.
    /// @return The new direction.
    math::Direction turnLeft();

    /// @brief Turn the player to the right.
    /// @return The new direction.
    math::Direction turnRight();

    void moveForward();

    /// @brief Get the player's inventory.
    /// @return The player's inventory.
    [[nodiscard]] const Inventory& inventory() const;

    void take(ResourceType resource);

    bool drop(ResourceType resource);

  private:
    /// @brief Eat one unit of food.
    /// @return True if the player ate food, false otherwise.
    bool eat();

    bool _alive{true};
    std::uint8_t _level{1};
    math::Direction _orientation{math::direction::random()};
    Inventory _inventory;
    std::optional<std::uint64_t> _foodTimerId;
};
}  // namespace zappy::server::game::entity
