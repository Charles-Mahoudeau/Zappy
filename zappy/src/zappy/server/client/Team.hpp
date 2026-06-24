/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Team
*/

#pragma once

#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "zappy/shared/network/Address.hpp"

namespace zappy::server::client {
class Team {
  public:
    explicit Team(std::string name);
    ~Team() = default;

    Team(const Team&) = default;
    Team& operator=(const Team&) = default;

    Team(Team&&) = default;
    Team& operator=(Team&&) = default;

    /// @brief Get the name of the team.
    /// @return The name of the team.
    [[nodiscard]] std::string_view name() const;

    /// @brief Get the addresses of the clients in the team.
    /// @return The addresses of the clients in the team.
    [[nodiscard]] std::span<const network::Address> members() const;

    /// @brief Check if the team has a client with the given address.
    /// @param address The address of the client to check.
    /// @return True if the team has a client with the given address, false otherwise.
    [[nodiscard]] bool hasMember(const network::Address& address) const;

    /// @brief Add a client address to the team.
    /// @param address The address of the client to add.
    void addMember(network::Address address);

  private:
    std::string _name;
    std::vector<network::Address> _clientAddresses;
};
}  // namespace zappy::server::client
