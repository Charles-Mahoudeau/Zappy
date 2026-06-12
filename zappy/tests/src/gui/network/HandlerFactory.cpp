/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** HandlerFactory tests
*/

#include "zappy/gui/network/HandlerFactory.hpp"

#include <gtest/gtest.h>

#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/shared/exception/ParseException.hpp"

using zappy::exception::ParseException;
using zappy::gui::game::GameState;
using zappy::gui::game::Orientation;
using zappy::gui::network::HandlerFactory;

namespace {
class HandlerFactoryTest : public ::testing::Test {
  public:
    GameState state;
    std::unordered_map<std::string, std::function<void(std::istringstream&)>> handlers = HandlerFactory::create(state);

  protected:
    void dispatch(const std::string& command, const std::string& args) {
        std::istringstream ss{args};
        handlers.at(command)(ss);
    }
};

TEST_F(HandlerFactoryTest, ContainsAllExpectedCommands) {
    for (const auto& cmd : {"msz", "bct", "tna", "pnw", "ppo", "plv", "pin", "pex", "pbc", "pic", "pie", "pfk",
                            "pdr", "pgt", "pdi", "enw", "ebo", "edi", "sgt", "sst", "seg", "smg", "suc", "sbp"}) {
        EXPECT_TRUE(handlers.count(cmd) == 1) << "missing handler: " << cmd;
    }
}

TEST_F(HandlerFactoryTest, MszSetsMapSize) {
    dispatch("msz", "10 5");
    EXPECT_EQ(state.width(), 10U);
    EXPECT_EQ(state.height(), 5U);
}

TEST_F(HandlerFactoryTest, BctSetsTileResources) {
    dispatch("msz", "5 5");
    dispatch("bct", "2 3 1 0 0 0 0 0 0");
    EXPECT_EQ(state.tile(2, 3).food, 1U);
}

TEST_F(HandlerFactoryTest, TnaAddsTeam) {
    dispatch("tna", "TeamA");
    ASSERT_EQ(state.teams().size(), 1U);
    EXPECT_EQ(state.teams().at(0), "TeamA");
}

TEST_F(HandlerFactoryTest, PnwAddsPlayer) {
    dispatch("msz", "10 10");
    dispatch("pnw", "#1 3 4 1 2 TeamA");
    ASSERT_TRUE(state.players().count(1));
    const auto& p = state.players().at(1);
    EXPECT_EQ(p.x, 3U);
    EXPECT_EQ(p.y, 4U);
    EXPECT_EQ(p.orientation, Orientation::North);
    EXPECT_EQ(p.level, 2U);
    EXPECT_EQ(p.team, "TeamA");
}

TEST_F(HandlerFactoryTest, PpoUpdatesPlayerPosition) {
    dispatch("msz", "10 10");
    dispatch("pnw", "#1 0 0 1 1 TeamA");
    dispatch("ppo", "#1 5 6 3");
    const auto& p = state.players().at(1);
    EXPECT_EQ(p.x, 5U);
    EXPECT_EQ(p.y, 6U);
    EXPECT_EQ(p.orientation, Orientation::South);
}

TEST_F(HandlerFactoryTest, PlvUpdatesPlayerLevel) {
    dispatch("msz", "10 10");
    dispatch("pnw", "#1 0 0 1 1 TeamA");
    dispatch("plv", "#1 5");
    EXPECT_EQ(state.players().at(1).level, 5U);
}

TEST_F(HandlerFactoryTest, PinUpdatesPlayerInventory) {
    dispatch("msz", "10 10");
    dispatch("pnw", "#2 0 0 1 1 TeamB");
    dispatch("pin", "#2 0 0 3 0 0 0 0 0 0");
    EXPECT_EQ(state.players().at(2).inventory.food, 3U);
}

TEST_F(HandlerFactoryTest, PexRemovesPlayer) {
    dispatch("msz", "10 10");
    dispatch("pnw", "#3 0 0 1 1 TeamA");
    dispatch("pex", "#3");
    EXPECT_FALSE(state.players().count(3));
}

TEST_F(HandlerFactoryTest, PdiRemovesPlayer) {
    dispatch("msz", "10 10");
    dispatch("pnw", "#4 0 0 1 1 TeamA");
    dispatch("pdi", "#4");
    EXPECT_FALSE(state.players().count(4));
}

TEST_F(HandlerFactoryTest, PbcAddsBroadcast) {
    dispatch("msz", "10 10");
    dispatch("pnw", "#1 0 0 1 1 TeamA");
    dispatch("pbc", "#1 hello world");
    ASSERT_EQ(state.broadcasts().size(), 1U);
    EXPECT_EQ(state.broadcasts().at(0), "hello world");
}

TEST_F(HandlerFactoryTest, PicSetsPlayersIncanting) {
    dispatch("msz", "10 10");
    dispatch("pnw", "#1 2 2 1 1 TeamA");
    dispatch("pnw", "#2 2 2 1 1 TeamA");
    dispatch("pic", "2 2 2 #1 #2");
    EXPECT_TRUE(state.players().at(1).isIncanting);
    EXPECT_TRUE(state.players().at(2).isIncanting);
}

TEST_F(HandlerFactoryTest, PieClearsIncantingAtTile) {
    dispatch("msz", "10 10");
    dispatch("pnw", "#1 2 2 1 1 TeamA");
    dispatch("pic", "2 2 2 #1");
    dispatch("pie", "2 2 1");
    EXPECT_FALSE(state.players().at(1).isIncanting);
}

TEST_F(HandlerFactoryTest, EnwAddsEgg) {
    dispatch("enw", "#10 #1 3 4");
    ASSERT_TRUE(state.eggs().count(10));
    const auto& e = state.eggs().at(10);
    EXPECT_EQ(e.playerId, 1U);
    EXPECT_EQ(e.x, 3U);
    EXPECT_EQ(e.y, 4U);
}

TEST_F(HandlerFactoryTest, EboRemovesEgg) {
    dispatch("enw", "#10 #1 3 4");
    dispatch("ebo", "#10");
    EXPECT_FALSE(state.eggs().count(10));
}

TEST_F(HandlerFactoryTest, EdiRemovesEgg) {
    dispatch("enw", "#11 #1 0 0");
    dispatch("edi", "#11");
    EXPECT_FALSE(state.eggs().count(11));
}

TEST_F(HandlerFactoryTest, SgtSetsTimeUnit) {
    dispatch("sgt", "100");
    EXPECT_EQ(state.timeUnit(), 100U);
}

TEST_F(HandlerFactoryTest, SstSetsTimeUnit) {
    dispatch("sst", "200");
    EXPECT_EQ(state.timeUnit(), 200U);
}

TEST_F(HandlerFactoryTest, SegSetsWinner) {
    dispatch("seg", "TeamA");
    ASSERT_TRUE(state.isGameOver());
    EXPECT_EQ(state.winner().value_or(""), "TeamA");
}

TEST_F(HandlerFactoryTest, SmgAddsBroadcast) {
    dispatch("smg", "server info message");
    ASSERT_EQ(state.broadcasts().size(), 1U);
    EXPECT_EQ(state.broadcasts().at(0), "server info message");
}

TEST_F(HandlerFactoryTest, SucDoesNothing) { EXPECT_NO_THROW(dispatch("suc", "")); }

TEST_F(HandlerFactoryTest, SbpDoesNothing) { EXPECT_NO_THROW(dispatch("sbp", "")); }

TEST_F(HandlerFactoryTest, MszMalformedThrows) { EXPECT_THROW(dispatch("msz", "10"), ParseException); }

TEST_F(HandlerFactoryTest, PnwBadIdThrows) { EXPECT_THROW(dispatch("pnw", "1 0 0 1 1 TeamA"), ParseException); }

TEST_F(HandlerFactoryTest, PpoInvalidOrientationThrows) {
    dispatch("msz", "10 10");
    dispatch("pnw", "#1 0 0 1 1 TeamA");
    EXPECT_THROW(dispatch("ppo", "#1 0 0 9"), ParseException);
}
}  // namespace
