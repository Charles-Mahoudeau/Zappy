/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** TeamRegistry
*/

#include "zappy/server/client/TeamRegistry.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <utility>
#include <vector>

#include "zappy/server/client/Team.hpp"
#include "zappy/shared/exception/InvalidArgument.hpp"

using namespace zappy::server::client;
using namespace zappy::network;

namespace {
// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

TEST(TeamRegistryConstructor, CreatesEmptyRegistry) {
    std::uint8_t count = 0;

    for (const TeamRegistry registry; const auto& _ : registry.teams()) {
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST(TeamRegistryConstructor, HasNoTeamNamesInitially) {
    std::uint8_t count = 0;

    for (const TeamRegistry registry; const auto& _ : registry.teamNames()) {
        count++;
    }
    EXPECT_EQ(count, 0);
}

// ---------------------------------------------------------------------------
// createTeam()
// ---------------------------------------------------------------------------

TEST(TeamRegistryCreateTeam, CreatesNewTeam) {
    TeamRegistry registry;
    const Team& team = registry.createTeam("Team1");

    EXPECT_EQ(team.name(), "Team1");

    std::uint8_t count = 0;

    for ([[maybe_unused]] auto t : registry.teams()) {
        count++;
    }
    EXPECT_EQ(count, 1);
}

TEST(TeamRegistryCreateTeam, ReturnsReferenceToCreatedTeam) {
    TeamRegistry registry;
    Team& team = registry.createTeam("Team2");

    team.addMember(Address{"127.0.0.1", 1000});

    const Team* foundTeam = registry.team("Team2");
    EXPECT_NE(foundTeam, nullptr);
    EXPECT_TRUE(foundTeam->hasMember(Address{"127.0.0.1", 1000}));
}

TEST(TeamRegistryCreateTeam, CreatesMultipleTeams) {
    TeamRegistry registry;
    registry.createTeam("TeamA");
    registry.createTeam("TeamB");
    registry.createTeam("TeamC");

    std::uint8_t count = 0;
    for ([[maybe_unused]] auto team : registry.teams()) {
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(TeamRegistryCreateTeam, ThrowsOnDuplicateNames) {
    TeamRegistry registry;
    registry.createTeam("Duplicate");

    EXPECT_THROW(registry.createTeam("Duplicate"), zappy::exception::InvalidArgument);
}

// ---------------------------------------------------------------------------
// team(const std::string& name)
// ---------------------------------------------------------------------------

TEST(TeamRegistryTeamByName, FindsExistingTeam) {
    TeamRegistry registry;
    registry.createTeam("TestTeam");

    const Team* team = registry.team("TestTeam");

    EXPECT_NE(team, nullptr);
    EXPECT_EQ(team->name(), "TestTeam");
}

TEST(TeamRegistryTeamByName, ReturnsNullptrForNonexistentTeam) {
    TeamRegistry registry;
    registry.createTeam("ExistingTeam");

    const Team* team = registry.team("NonexistentTeam");

    EXPECT_EQ(team, nullptr);
}

TEST(TeamRegistryTeamByName, ReturnsNullptrForEmptyRegistry) {
    const TeamRegistry registry;
    const Team* team = registry.team("AnyTeam");

    EXPECT_EQ(team, nullptr);
}

TEST(TeamRegistryTeamByName, FindsCorrectTeamAmongMultiple) {
    TeamRegistry registry;

    registry.createTeam("Alpha");
    registry.createTeam("Beta");
    registry.createTeam("Gamma");

    const Team* team = registry.team("Beta");

    EXPECT_NE(team, nullptr);
    EXPECT_EQ(team->name(), "Beta");
}

TEST(TeamRegistryTeamByName, IsCaseSensitive) {
    TeamRegistry registry;

    registry.createTeam("TestTeam");

    const Team* team1 = registry.team("TestTeam");
    const Team* team2 = registry.team("testteam");

    EXPECT_NE(team1, nullptr);
    EXPECT_EQ(team2, nullptr);
}

// ---------------------------------------------------------------------------
// team(const Address& address)
// ---------------------------------------------------------------------------

TEST(TeamRegistryTeamByAddress, FindsTeamByMemberAddress) {
    TeamRegistry registry;
    Team& team = registry.createTeam("WithMembers");
    const Address memberAddr("192.168.1.100", 5000);

    team.addMember(memberAddr);

    const Team* found = registry.team(memberAddr);

    EXPECT_NE(found, nullptr);
    EXPECT_EQ(found->name(), "WithMembers");
}

TEST(TeamRegistryTeamByAddress, ReturnsNullptrForNonexistentAddress) {
    TeamRegistry registry;
    Team& team = registry.createTeam("Team1");
    team.addMember(Address("192.168.1.100", 5000));

    const Team* found = registry.team(Address("192.168.1.101", 5001));

    EXPECT_EQ(found, nullptr);
}

TEST(TeamRegistryTeamByAddress, ReturnsNullptrWhenNoTeamsExist) {
    const TeamRegistry registry;

    const Team* found = registry.team(Address("127.0.0.1", 1234));

    EXPECT_EQ(found, nullptr);
}

TEST(TeamRegistryTeamByAddress, FindsCorrectTeamWithMultipleMembers) {
    TeamRegistry registry;
    Team& team1 = registry.createTeam("First");
    Team& team2 = registry.createTeam("Second");

    const Address addr1("10.0.0.1", 1000);
    const Address addr2("10.0.0.2", 2000);
    const Address addr3("10.0.0.3", 3000);

    team1.addMember(addr1);
    team1.addMember(addr2);
    team2.addMember(addr3);

    EXPECT_EQ(registry.team(addr1)->name(), "First");
    EXPECT_EQ(registry.team(addr2)->name(), "First");
    EXPECT_EQ(registry.team(addr3)->name(), "Second");
}

TEST(TeamRegistryTeamByAddress, ThrowsOnDuplicateMembers) {
    TeamRegistry registry;
    Team& team = registry.createTeam("DuplicateMembers");
    const Address addr{"127.0.0.1", 9000};

    team.addMember(addr);

    EXPECT_THROW(team.addMember(addr), zappy::exception::InvalidArgument);
}

// ---------------------------------------------------------------------------
// teams()
// ---------------------------------------------------------------------------

TEST(TeamRegistryTeams, ReturnsViewOfAllTeams) {
    TeamRegistry registry;
    registry.createTeam("Team1");
    registry.createTeam("Team2");
    registry.createTeam("Team3");

    auto teamsView = registry.teams();
    std::uint8_t count = 0;

    for ([[maybe_unused]] auto team : teamsView) {
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(TeamRegistryTeams, ReturnsEmptyViewForEmptyRegistry) {
    const TeamRegistry registry;
    auto teamsView = registry.teams();
    std::uint8_t count = 0;

    for ([[maybe_unused]] auto team : teamsView) {
        count++;
    }
    EXPECT_EQ(count, 0);
}

// ---------------------------------------------------------------------------
// teamNames()
// ---------------------------------------------------------------------------

TEST(TeamRegistryTeamNames, ReturnsViewOfAllTeamNames) {
    TeamRegistry registry;

    registry.createTeam("Alpha");
    registry.createTeam("Bravo");
    registry.createTeam("Charlie");

    auto namesView = registry.teamNames();
    std::uint8_t count = 0;

    for ([[maybe_unused]] auto name : namesView) {
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(TeamRegistryTeamNames, ReturnsEmptyViewForEmptyRegistry) {
    const TeamRegistry registry;
    auto namesView = registry.teamNames();
    std::uint8_t count = 0;

    for ([[maybe_unused]] auto name : namesView) {
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST(TeamRegistryTeamNames, ContainsCorrectTeamNames) {
    TeamRegistry registry;
    registry.createTeam("First");
    registry.createTeam("Second");
    registry.createTeam("Third");

    auto namesView = registry.teamNames();
    std::vector<std::string> names;

    names.reserve(std::ranges::distance(namesView));
    for (const auto& name : namesView) {
        names.emplace_back(name);
    }
    EXPECT_EQ(names.size(), 3);
    EXPECT_TRUE(std::ranges::find(names, "First") != names.end());
    EXPECT_TRUE(std::ranges::find(names, "Second") != names.end());
    EXPECT_TRUE(std::ranges::find(names, "Third") != names.end());
}

// ---------------------------------------------------------------------------
// addToTeam()
// ---------------------------------------------------------------------------

TEST(TeamRegistryAddToTeam, AddsAddressToExistingTeam) {
    TeamRegistry registry;

    registry.createTeam("TargetTeam");

    const Address addr{"127.0.0.1", 7000};

    registry.addToTeam("TargetTeam", addr);

    const Team* team = registry.team("TargetTeam");
    EXPECT_TRUE(team->hasMember(addr));
}

TEST(TeamRegistryAddToTeam, AddsMultipleAddressesToTeam) {
    TeamRegistry registry;

    registry.createTeam("MultiMemberTeam");

    const Address addr1{"192.168.1.1", 1000};
    const Address addr2{"192.168.1.2", 2000};
    const Address addr3{"192.168.1.3", 3000};

    registry.addToTeam("MultiMemberTeam", addr1);
    registry.addToTeam("MultiMemberTeam", addr2);
    registry.addToTeam("MultiMemberTeam", addr3);

    const Team* team = registry.team("MultiMemberTeam");
    EXPECT_EQ(team->members().size(), 3);
    EXPECT_TRUE(team->hasMember(addr1));
    EXPECT_TRUE(team->hasMember(addr2));
    EXPECT_TRUE(team->hasMember(addr3));
}

TEST(TeamRegistryAddToTeam, FindsTeamByAddressAfterAdding) {
    TeamRegistry registry;

    registry.createTeam("SearchableTeam");

    const Address addr{"10.0.0.50", 8080};

    registry.addToTeam("SearchableTeam", addr);

    const Team* foundByAddr = registry.team(addr);
    EXPECT_NE(foundByAddr, nullptr);
    EXPECT_EQ(foundByAddr->name(), "SearchableTeam");
}

TEST(TeamRegistryAddToTeam, ThrowsOnDuplicateAddresses) {
    TeamRegistry registry;

    registry.createTeam("TeamWithDupes");

    const Address addr{"127.0.0.1", 3000};

    registry.addToTeam("TeamWithDupes", addr);

    EXPECT_THROW(registry.addToTeam("TeamWithDupes", addr), zappy::exception::InvalidArgument);
}

// ---------------------------------------------------------------------------
// Copy and Move semantics
// ---------------------------------------------------------------------------

TEST(TeamRegistryCopySemantics, CopiesRegistryCorrectly) {
    TeamRegistry original;

    original.createTeam("Team1");
    original.createTeam("Team2");
    original.addToTeam("Team1", Address{"127.0.0.1", 1000});

    const TeamRegistry copy = original;

    EXPECT_NE(copy.team("Team1"), nullptr);
    EXPECT_NE(copy.team("Team2"), nullptr);
    EXPECT_TRUE(copy.team("Team1")->hasMember(Address{"127.0.0.1", 1000}));
}

TEST(TeamRegistryMoveSemantics, MovesRegistryCorrectly) {
    TeamRegistry original;

    original.createTeam("MovedTeam");
    original.addToTeam("MovedTeam", Address{"127.0.0.1", 5000});

    const TeamRegistry moved = std::move(original);

    EXPECT_NE(moved.team("MovedTeam"), nullptr);
    EXPECT_TRUE(moved.team("MovedTeam")->hasMember(Address{"127.0.0.1", 5000}));
}

TEST(TeamRegistryAssignmentSemantics, AssignsRegistryCorrectly) {
    TeamRegistry registry1;

    registry1.createTeam("Team1");

    TeamRegistry registry2;

    registry2.createTeam("Team2");
    registry2.addToTeam("Team2", Address{"127.0.0.1", 6000});
    registry1 = registry2;
    EXPECT_EQ(registry1.team("Team1"), nullptr);
    EXPECT_NE(registry1.team("Team2"), nullptr);
    EXPECT_TRUE(registry1.team("Team2")->hasMember(Address{"127.0.0.1", 6000}));
}
}  // namespace
