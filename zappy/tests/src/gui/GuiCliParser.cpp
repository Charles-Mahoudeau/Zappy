/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GuiCliParser tests
*/

#include "zappy/gui/GuiCliParser.hpp"

#include <gtest/gtest.h>

#include <string_view>
#include <vector>

#include "zappy/shared/exception/InvalidArgument.hpp"

using zappy::exception::InvalidArgument;
using zappy::gui::GuiCliParser;

namespace {

GuiCliParser make(std::vector<std::string_view> args) { return GuiCliParser{args}; }

TEST(GuiCliParserTest, ParsesHostAndPort) {
    const auto p = make({"-h", "localhost", "-p", "4242"});
    EXPECT_EQ(p.host(), "localhost");
    EXPECT_EQ(p.port(), 4242);
}

TEST(GuiCliParserTest, ParsesInReverseOrder) {
    const auto p = make({"-p", "4242", "-h", "127.0.0.1"});
    EXPECT_EQ(p.host(), "127.0.0.1");
    EXPECT_EQ(p.port(), 4242);
}

TEST(GuiCliParserTest, MissingHostThrows) { EXPECT_THROW(make({"-p", "4242"}), InvalidArgument); }

TEST(GuiCliParserTest, MissingPortThrows) { EXPECT_THROW(make({"-h", "localhost"}), InvalidArgument); }

TEST(GuiCliParserTest, InvalidPortThrows) { EXPECT_THROW(make({"-h", "localhost", "-p", "abc"}), InvalidArgument); }

TEST(GuiCliParserTest, PortOutOfRangeThrows) {
    EXPECT_THROW(make({"-h", "localhost", "-p", "99999"}), InvalidArgument);
}

TEST(GuiCliParserTest, EmptyArgsThrows) { EXPECT_THROW(make({}), InvalidArgument); }

}  // namespace
