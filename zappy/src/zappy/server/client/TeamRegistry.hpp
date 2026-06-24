/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TeamRegistry
*/

#pragma once

#include <beman/any_view/any_view.hpp>
#include <string>
#include <unordered_map>

#include "Team.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server::client {
class TeamRegistry {
  public:
    TeamRegistry() = default;
    ~TeamRegistry() = default;

    TeamRegistry(const TeamRegistry&) = default;
    TeamRegistry& operator=(const TeamRegistry&) = default;

    TeamRegistry(TeamRegistry&&) = default;
    TeamRegistry& operator=(TeamRegistry&&) = default;

    /// @brief Returns a view of all teams.
    /// @return A view of all teams.
    [[nodiscard]] beman::any_view::any_view<const Team> teams() const;

    /// @brief Returns a view of all teams.
    /// @return A view of all teams.
    [[nodiscard]] beman::any_view::any_view<Team> teams();

    /// @brief Returns a team with the given name.
    /// @param name The name of the team to return.
    /// @return A pointer to the team with the given name, or nullptr if no such team exists.
    [[nodiscard]] const Team* team(const std::string& name) const;

    /// @brief Returns a team with the given address.
    /// @param address The address of the team to return.
    /// @return A pointer to the team with the given address, or nullptr if no such team exists.
    [[nodiscard]] const Team* team(const network::Address& address) const;

    /// @brief Returns a view of all team names.
    /// @return A view of all team names.
    [[nodiscard]] beman::any_view::any_view<const std::string> teamNames() const;

    /// @brief Creates a new team with the given name.
    /// @param name The name of the team to create.
    /// @return A reference to the newly created team.
    Team& createTeam(std::string name);

  private:
    std::unordered_map<std::string, Team> _teams;
};
}  // namespace zappy::server::client
