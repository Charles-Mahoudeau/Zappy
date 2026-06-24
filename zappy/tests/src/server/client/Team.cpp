/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Team
*/

#include "zappy/server/client/Team.hpp"

#include <gtest/gtest.h>

#include <utility>

#include "zappy/shared/exception/InvalidArgument.hpp"

using namespace zappy::server::client;
using namespace zappy::network;

namespace {

// ---------------------------------------------------------------------------
// Constructor
// ---------------------------------------------------------------------------

TEST(TeamConstructor, CreatesTeamWithName) {
    const Team team{"Alpha"};

    EXPECT_EQ(team.name(), "Alpha");
}

TEST(TeamConstructor, HasNoMembersInitially) {
    const Team team{"Bravo"};

    EXPECT_EQ(team.members().size(), 0);
}

TEST(TeamConstructor, ThrowOnEmptyName) { EXPECT_THROW(Team{""}, zappy::exception::InvalidArgument); }

// ---------------------------------------------------------------------------
// name()
// ---------------------------------------------------------------------------

TEST(TeamName, ReturnsCorrectName) {
    const Team team{"Delta"};

    EXPECT_EQ(team.name(), "Delta");
}

// ---------------------------------------------------------------------------
// members()
// ---------------------------------------------------------------------------

TEST(TeamMembers, ReturnsEmptySpanInitially) {
    const Team team{"Foxtrot"};

    EXPECT_EQ(team.members().size(), 0);
}

TEST(TeamMembers, CanIterateMembers) {
    Team team{"Hotel"};

    team.addMember(Address{"127.0.0.1", 1234});
    team.addMember(Address{"127.0.0.2", 5678});

    const auto members = team.members();

    EXPECT_EQ(members.size(), 2);
    // NOLINTBEGIN(*-pro-bounds-avoid-unchecked-container-access)
    EXPECT_EQ(members[0], (Address{"127.0.0.1", 1234}));
    EXPECT_EQ(members[1], (Address{"127.0.0.2", 5678}));
    // NOLINTEND(*-pro-bounds-avoid-unchecked-container-access)
}

// ---------------------------------------------------------------------------
// hasMember()
// ---------------------------------------------------------------------------

TEST(TeamHasMember, ReturnsTrueForExistingMember) {
    Team team{"India"};
    const Address addr{"192.168.1.1", 8080};

    team.addMember(addr);

    EXPECT_TRUE(team.hasMember(addr));
}

TEST(TeamHasMember, ReturnsFalseForNonexistentMember) {
    Team team{"Juliet"};
    const Address addr1{"192.168.1.1", 8080};
    const Address addr2{"192.168.1.2", 8081};
    team.addMember(addr1);

    EXPECT_FALSE(team.hasMember(addr2));
}

TEST(TeamHasMember, ReturnsFalseForEmptyTeam) {
    const Team team{"Kilo"};

    EXPECT_FALSE(team.hasMember(Address{"127.0.0.1", 1234}));
}

TEST(TeamHasMember, ChecksMultipleMembers) {
    Team team{"Lima"};
    const Address addr1{"10.0.0.1", 1000};
    const Address addr2{"10.0.0.2", 2000};
    const Address addr3{"10.0.0.3", 3000};

    team.addMember(addr1);
    team.addMember(addr2);

    EXPECT_TRUE(team.hasMember(addr1));
    EXPECT_TRUE(team.hasMember(addr2));
    EXPECT_FALSE(team.hasMember(addr3));
}

// ---------------------------------------------------------------------------
// addMember()
// ---------------------------------------------------------------------------

TEST(TeamAddMember, AddsNewMember) {
    Team team{"Mike"};
    const Address addr{"127.0.0.1", 4000};

    team.addMember(addr);

    EXPECT_TRUE(team.hasMember(addr));
    EXPECT_EQ(team.members().size(), 1);
}

TEST(TeamAddMember, AddsMultipleMembers) {
    Team team{"November"};
    const Address addr1{"127.0.0.1", 4000};
    const Address addr2{"127.0.0.2", 4001};
    const Address addr3{"127.0.0.3", 4002};

    team.addMember(addr1);
    team.addMember(addr2);
    team.addMember(addr3);

    EXPECT_EQ(team.members().size(), 3);
    EXPECT_TRUE(team.hasMember(addr1));
    EXPECT_TRUE(team.hasMember(addr2));
    EXPECT_TRUE(team.hasMember(addr3));
}

TEST(TeamAddMember, ThrowsOnDuplicateMembers) {
    Team team{"Oscar"};
    const Address addr{"127.0.0.1", 5000};

    team.addMember(addr);

    EXPECT_THROW(team.addMember(addr), zappy::exception::InvalidArgument);
}

// ---------------------------------------------------------------------------
// Copy and Move semantics
// ---------------------------------------------------------------------------

TEST(TeamCopySemantics, CopiesTeamCorrectly) {
    Team original{"Papa"};
    original.addMember(Address{"127.0.0.1", 6000});
    original.addMember(Address{"127.0.0.2", 6001});

    const Team copy = original;

    EXPECT_EQ(copy.name(), original.name());
    EXPECT_EQ(copy.members().size(), original.members().size());
    EXPECT_TRUE(copy.hasMember(Address{"127.0.0.1", 6000}));
    EXPECT_TRUE(copy.hasMember(Address{"127.0.0.2", 6001}));
}

TEST(TeamMoveSemantics, MovesTeamCorrectly) {
    Team original{"Quebec"};
    original.addMember(Address{"127.0.0.1", 7000});
    original.addMember(Address{"127.0.0.2", 7001});

    const Team moved = std::move(original);

    EXPECT_EQ(moved.name(), "Quebec");
    EXPECT_EQ(moved.members().size(), 2);
    EXPECT_TRUE(moved.hasMember(Address{"127.0.0.1", 7000}));
    EXPECT_TRUE(moved.hasMember(Address{"127.0.0.2", 7001}));
}

TEST(TeamAssignmentSemantics, AssignsTeamCorrectly) {
    Team team1{"Romeo"};
    team1.addMember(Address{"127.0.0.1", 8000});

    Team team2{"Sierra"};
    team2.addMember(Address{"127.0.0.2", 8001});

    team1 = team2;

    EXPECT_EQ(team1.name(), "Sierra");
    EXPECT_EQ(team1.members().size(), 1);
    EXPECT_TRUE(team1.hasMember(Address("127.0.0.2", 8001)));
}

}  // namespace
