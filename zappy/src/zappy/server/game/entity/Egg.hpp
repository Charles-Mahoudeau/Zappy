/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Egg
*/

#pragma once

#include <string>
#include <string_view>

#include "zappy/server/game/IEntity.hpp"

namespace zappy::server::game::entity {
class Egg : public IEntity {
  public:
    explicit Egg(std::string teamName);
    ~Egg() override = default;

    Egg(const Egg&) = default;
    Egg& operator=(const Egg&) = default;

    Egg(Egg&&) = default;
    Egg& operator=(Egg&&) = default;

    /// @brief Update the egg.
    void update() override;

    /// @brief Get the team name of the egg.
    /// @return The team name of the egg.
    [[nodiscard]] std::string_view teamName() const;

  private:
    std::string _teamName;
};
}  // namespace zappy::server::game::entity
