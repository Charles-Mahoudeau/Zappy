/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Team
*/

#include "Team.hpp"

#include <algorithm>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "zappy/shared/network/Address.hpp"

namespace zappy::server::client {
Team::Team(std::string name) : _name{std::move(name)} {}

std::string_view Team::name() const { return _name; }

std::span<const network::Address> Team::clientAddresses() const { return _clientAddresses; }

bool Team::hasClientAddress(const network::Address& address) const {
    return std::ranges::contains(_clientAddresses, address);
}

void Team::addClientAddress(network::Address address) { _clientAddresses.emplace_back(address); }
}  // namespace zappy::server::client
