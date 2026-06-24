/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TeamRegistry
*/

#include "TeamRegistry.hpp"

#include <algorithm>
#include <beman/any_view/any_view.hpp>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>

#include "Team.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server::client {
beman::any_view::any_view<const Team> TeamRegistry::teams() const { return _teams | std::views::values; }

const Team* TeamRegistry::team(const std::string& name) const {
    if (const auto it = _teams.find(name); it != _teams.end()) {
        return &it->second;
    }
    return nullptr;
}

const Team* TeamRegistry::team(const network::Address& address) const {
    auto teams = _teams | std::views::values;
    const auto it = std::ranges::find_if(teams, [address](const Team& team) { return team.hasMember(address); });

    if (it == teams.end()) {
        return nullptr;
    }
    return &*it;
}

beman::any_view::any_view<const std::string> TeamRegistry::teamNames() const { return _teams | std::views::keys; }

Team& TeamRegistry::createTeam(const std::string_view name) {
    if (auto [it, inserted] = _teams.try_emplace(std::string{name}, std::string{name}); inserted) {
        return it->second;
    }
    throw exception::InvalidArgument{"team name already in use"};
}

void TeamRegistry::addToTeam(const std::string& teamName, const network::Address& address) {
    const auto it = _teams.find(teamName);

    if (it == _teams.end()) {
        throw exception::InvalidArgument{"team does not exist"};
    }
    it->second.addMember(address);
}
}  // namespace zappy::server::client
