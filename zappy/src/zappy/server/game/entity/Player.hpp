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
#include <tuple>
#include <unordered_map>

#include "zappy/server/game/IEntity.hpp"
#include "zappy/server/game/Inventory.hpp"

namespace zappy::server::game::entity {
class Player : public IEntity {
  public:
    enum class Direction : std::uint8_t {
        kNorth = 0,
        kSouth = 1,
        kEast = 2,
        kWest = 3,
    };

    static constexpr std::uint8_t kMaxLevel{8};
    static constexpr std::uint8_t kDefaultLifeUnits{10};
    static constexpr std::uint8_t kTimeUnitsPerFood{126};
    static constexpr std::uint16_t kDefaultLifetime{kDefaultLifeUnits * kTimeUnitsPerFood};

    Player() = default;
    ~Player() override = default;

    Player(const Player&) = default;
    Player& operator=(const Player&) = default;

    Player(Player&&) = default;
    Player& operator=(Player&&) = default;

    /// @brief Update the player.
    void update() override;

    /// @brief Get the player's lifetime left.
    /// @return The player's lifetime left.
    [[nodiscard]] std::uint32_t lifetimeLeft() const;

    /// @brief Check if the player is alive.
    /// @return True if the player is alive, false otherwise.
    [[nodiscard]] bool alive() const;

    /// @brief Kill the player.
    void kill();

    /// @brief Check if the player is frozen.
    /// @return True if the player is frozen, false otherwise.
    [[nodiscard]] bool frozen() const;

    /// @brief Freeze the player.
    /// @param time The time in seconds the player will be frozen.
    void freeze(std::uint32_t time);

    /// @brief Get the player's level.
    /// @return The player's level.
    [[nodiscard]] std::uint8_t level() const;

    /// @brief Increase the player's level.
    /// @return The new level of the player.
    std::expected<std::uint8_t, std::string> levelUp();

    /// @brief Get the player's direction.
    /// @return The player's direction.
    [[nodiscard]] Direction direction() const;

    /// @brief Turn the player to the left.
    /// @return The new direction.
    Direction turnLeft();

    /// @brief Turn the player to the right.
    /// @return The new direction.
    Direction turnRight();

    /// @brief Get the player's inventory.
    /// @return The player's inventory.
    [[nodiscard]] Inventory& inventory();

    /// @brief Get the player's inventory.
    /// @return The player's inventory.
    [[nodiscard]] const Inventory& inventory() const;

    /// @brief Eat one unit of food.
    /// @return True if the player ate food, false otherwise.
    bool eat();

  private:
    /// @brief Get the turn map.
    /// @return The turn map. The key is the current direction, and the value is a tuple of the left and right
    /// directions.
    [[nodiscard]] static const std::unordered_map<Direction, std::tuple<Direction, Direction>>& turnMap();

    std::uint32_t _lifetimeLeft{kDefaultLifetime};
    std::uint8_t _level{1};
    std::uint32_t _freezeTime{0};
    Direction _direction{Direction::kNorth};
    Inventory _inventory;
};
}  // namespace zappy::server::game::entity
