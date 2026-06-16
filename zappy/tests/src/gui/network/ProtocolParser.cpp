/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ProtocolParser tests
*/

#include "zappy/gui/network/ProtocolParser.hpp"

#include <gtest/gtest.h>

#include <type_traits>

#include "zappy/gui/game/GameState.hpp"

namespace {
using zappy::gui::game::GameState;
using zappy::gui::network::ProtocolParser;

class ProtocolParserTest : public ::testing::Test {
  public:
    GameState state;
    ProtocolParser parser{state};
};

TEST_F(ProtocolParserTest, DispatchMszUpdatesMapSize) {
    parser.dispatch("msz 10 5");
    EXPECT_EQ(state.width(), 10U);
    EXPECT_EQ(state.height(), 5U);
}

TEST_F(ProtocolParserTest, DispatchTnaAddsTeam) {
    parser.dispatch("tna TeamA");
    ASSERT_EQ(state.teams().size(), 1U);
    EXPECT_EQ(state.teams().at(0), "TeamA");
}

TEST_F(ProtocolParserTest, DispatchPnwAddsPlayer) {
    parser.dispatch("msz 10 10");
    parser.dispatch("pnw #1 3 4 1 2 TeamA");
    ASSERT_TRUE(state.players().count(1));
    EXPECT_EQ(state.players().at(1).x, 3U);
}

TEST_F(ProtocolParserTest, DispatchSgtSetsTimeUnit) {
    parser.dispatch("sgt 100");
    EXPECT_EQ(state.timeUnit(), 100U);
}

TEST_F(ProtocolParserTest, DispatchSegSetsWinner) {
    parser.dispatch("seg TeamA");
    ASSERT_TRUE(state.winner().has_value());
    EXPECT_EQ(state.winner().value_or(""), "TeamA");
}

TEST_F(ProtocolParserTest, UnknownCommandSilentlyIgnored) { EXPECT_NO_THROW(parser.dispatch("xyz 1 2 3")); }

TEST_F(ProtocolParserTest, EmptyLineSilentlyIgnored) { EXPECT_NO_THROW(parser.dispatch("")); }

TEST_F(ProtocolParserTest, WhitespaceOnlyLineSilentlyIgnored) { EXPECT_NO_THROW(parser.dispatch("   ")); }

TEST_F(ProtocolParserTest, MalformedArgumentsSilentlyIgnored) { EXPECT_NO_THROW(parser.dispatch("msz broken")); }

TEST_F(ProtocolParserTest, MalformedIdSilentlyIgnored) { EXPECT_NO_THROW(parser.dispatch("pnw 1 0 0 1 1 TeamA")); }

TEST_F(ProtocolParserTest, MultipleDispatchesInSequence) {
    parser.dispatch("msz 5 5");
    parser.dispatch("tna TeamA");
    parser.dispatch("tna TeamB");
    parser.dispatch("sgt 42");
    EXPECT_EQ(state.width(), 5U);
    EXPECT_EQ(state.teams().size(), 2U);
    EXPECT_EQ(state.timeUnit(), 42U);
}

TEST_F(ProtocolParserTest, DispatchIsMoveOnly) {
    EXPECT_FALSE(std::is_copy_constructible_v<ProtocolParser>);
    EXPECT_FALSE(std::is_copy_assignable_v<ProtocolParser>);
    EXPECT_TRUE(std::is_move_constructible_v<ProtocolParser>);
    EXPECT_TRUE(std::is_move_assignable_v<ProtocolParser>);
}
}  // namespace
