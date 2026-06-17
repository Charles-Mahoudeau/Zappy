/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** CommandSender tests
*/

#include "zappy/gui/network/CommandSender.hpp"

#include <gtest/gtest.h>

#include <type_traits>

#include "zappy/shared/exception/InvalidArgument.hpp"
#include "zappy/shared/network/BufferedClient.hpp"

using zappy::exception::InvalidArgument;
using zappy::gui::network::CommandSender;
using zappy::network::BufferedClient;

namespace {
class CommandSenderTest : public ::testing::Test {
  public:
    BufferedClient buffer;
    CommandSender sender{buffer};
};

TEST_F(CommandSenderTest, IsMoveOnly) {
    EXPECT_FALSE(std::is_copy_constructible_v<CommandSender>);
    EXPECT_FALSE(std::is_copy_assignable_v<CommandSender>);
    EXPECT_TRUE(std::is_move_constructible_v<CommandSender>);
    EXPECT_TRUE(std::is_move_assignable_v<CommandSender>);
}

TEST_F(CommandSenderTest, SetTimeUnitThrowsOnZero) { EXPECT_THROW(sender.setTimeUnit(0), InvalidArgument); }

TEST_F(CommandSenderTest, SetTimeUnitThrowsOnNegative) {
    EXPECT_THROW(sender.setTimeUnit(-1), InvalidArgument);
    EXPECT_THROW(sender.setTimeUnit(-100), InvalidArgument);
}
}  // namespace
