/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Cli Parsing Test
*/

#include "zappy/server/CliParser.hpp"

#include <gtest/gtest.h>

#include <string_view>
#include <vector>

#include "zappy/shared/exception/InvalidArgument.hpp"

using Params = zappy::server::CliParser::CliParameter;
using CLI = zappy::server::CliParser;
using InvalidArg = zappy::exception::InvalidArgument;

namespace {

std::vector<std::string_view> makeFullArgv(std::string_view port = "4242", std::string_view width = "10",
                                           std::string_view height = "10",
                                           const std::vector<std::string_view>& teams = {"teamA", "teamB"},
                                           std::string_view clients = "5", std::string_view freq = "100") {
    std::vector<std::string_view> argv = {"-p", port, "-x", width, "-y", height, "-n"};
    for (const auto& t : teams) {
        argv.push_back(t);
    }
    argv.insert(argv.end(), {"-c", clients, "-f", freq});
    return argv;
}

}  // namespace

// ─────────────────────────────────────────────
//  Happy path
// ─────────────────────────────────────────────

TEST(CliParser, valid_full_arguments_parsed_correctly) {
    auto params = CLI::parseArguments(makeFullArgv());
    ASSERT_NO_THROW(CLI::checkArgumentsValidity(params));

    EXPECT_EQ(params.port, 4242);
    EXPECT_EQ(params.mapWidth, 10U);
    EXPECT_EQ(params.mapHeight, 10U);
    EXPECT_EQ(params.nbInitialClient, 5U);
    EXPECT_EQ(params.frequencies, 100U);
    ASSERT_EQ(params.teamsName.size(), 2U);
    EXPECT_EQ(params.teamsName.at(0), "teamA");
    EXPECT_EQ(params.teamsName.at(1), "teamB");
}

TEST(CliParser, flags_in_different_order_parsed_correctly) {
    const std::vector<std::string_view> data = {"-f", "50", "-c", "3",  "-n", "red", "blue",
                                                "-y", "15", "-x", "25", "-p", "1234"};
    auto params = CLI::parseArguments(data);
    EXPECT_EQ(params.port, 1234);
    EXPECT_EQ(params.mapWidth, 25U);
    EXPECT_EQ(params.mapHeight, 15U);
    EXPECT_EQ(params.frequencies, 50U);
    EXPECT_EQ(params.nbInitialClient, 3U);
    ASSERT_EQ(params.teamsName.size(), 2U);
}

TEST(CliParser, teams_flag_block_single_team) {
    auto data = makeFullArgv("4242", "10", "10", {"onlyTeam"});
    auto params = CLI::parseArguments(data);
    ASSERT_THROW(CLI::checkArgumentsValidity(params), InvalidArg);
}

TEST(CliParser, empty_argv_returns_default_params) {
    auto params = CLI::parseArguments({});

    ASSERT_THROW(CLI::checkArgumentsValidity(params), InvalidArg);
}

// ─────────────────────────────────────────────
//  Malformed flags
// ─────────────────────────────────────────────

TEST(CliParser, unknown_flag_throws) { ASSERT_THROW(CLI::parseArguments({"-z", "42"}), InvalidArg); }

TEST(CliParser, no_leading_dash_throws) { ASSERT_THROW(CLI::parseArguments({"p", "4242"}), InvalidArg); }

TEST(CliParser, double_dash_flag_throws) { ASSERT_THROW(CLI::parseArguments({"--port", "4242"}), InvalidArg); }

TEST(CliParser, only_dash_flag_throws) { ASSERT_THROW(CLI::parseArguments({"-", "4242"}), InvalidArg); }

// ─────────────────────────────────────────────
//  Parameter count
// ─────────────────────────────────────────────

TEST(CliParser, flag_without_required_parameter_throws) { ASSERT_THROW(CLI::parseArguments({"-p"}), InvalidArg); }

TEST(CliParser, flag_with_too_many_parameters_throws) {
    ASSERT_THROW(CLI::parseArguments({"-p", "4242", "9999"}), InvalidArg);
}

// ─────────────────────────────────────────────
//  Value parsing errors
// ─────────────────────────────────────────────

TEST(CliParser, non_numeric_value_throws) { ASSERT_THROW(CLI::parseArguments({"-p", "abc"}), InvalidArg); }

TEST(CliParser, empty_value_throws) {
    // "-p" followed by "" — but "" doesn't start with '-' so it's a param
    ASSERT_THROW(CLI::parseArguments({"-p", ""}), InvalidArg);
}

TEST(CliParser, partial_numeric_value_throws) {
    // stoull would parse "42" then fail on "abc"
    ASSERT_THROW(CLI::parseArguments({"-p", "42abc"}), InvalidArg);
}

TEST(CliParser, negative_value_throws) {
    // '-1' is interpreted as a flag, so -p ends up with 0 params → throws
    ASSERT_THROW(CLI::parseArguments({"-p", "-1"}), InvalidArg);
}

// ─────────────────────────────────────────────
//  Overflow handling
// ─────────────────────────────────────────────

TEST(CliParser, uint16_field_above_max_throws) {
    // port is uint16_t (max 65535)
    ASSERT_THROW(CLI::parseArguments({"-p", "65536"}), InvalidArg);
}

TEST(CliParser, uint16_field_at_max_accepted) {
    auto params = CLI::parseArguments({"-p", "65535"});
    EXPECT_EQ(params.port, 65535);
}

TEST(CliParser, uint32_field_above_max_throws) {
    ASSERT_THROW(CLI::parseArguments({"-x", "99999999999999999999999999"}), InvalidArg);
}

// ─────────────────────────────────────────────
//  checkArgumentsValidity
// ─────────────────────────────────────────────

// One parameterized-style test per field; each exercises a distinct branch
// of the condition list, so they are NOT duplicates.

TEST(CliParser, validity_rejects_zero_port) {
    auto p = CLI::parseArguments(makeFullArgv("0"));
    ASSERT_THROW(CLI::checkArgumentsValidity(p), InvalidArg);
}

TEST(CliParser, validity_rejects_zero_width) {
    auto p = CLI::parseArguments(makeFullArgv("4242", "0"));
    ASSERT_THROW(CLI::checkArgumentsValidity(p), InvalidArg);
}

TEST(CliParser, validity_rejects_zero_height) {
    auto p = CLI::parseArguments(makeFullArgv("4242", "10", "0"));
    ASSERT_THROW(CLI::checkArgumentsValidity(p), InvalidArg);
}

TEST(CliParser, validity_rejects_zero_clients) {
    auto p = CLI::parseArguments(makeFullArgv("4242", "10", "10", {"teamA", "teamB"}, "0"));
    ASSERT_THROW(CLI::checkArgumentsValidity(p), InvalidArg);
}

TEST(CliParser, validity_rejects_zero_frequency) {
    auto p = CLI::parseArguments(makeFullArgv("4242", "10", "10", {"teamA", "teamB"}, "5", "0"));
    ASSERT_THROW(CLI::checkArgumentsValidity(p), InvalidArg);
}

TEST(CliParser, validity_rejects_empty_teams) {
    // -n with no following words → empty teamsName vector
    auto p = CLI::parseArguments(makeFullArgv("4242", "10", "10", std::vector<std::string_view>{}));
    EXPECT_TRUE(p.teamsName.empty());
    ASSERT_THROW(CLI::checkArgumentsValidity(p), InvalidArg);
}
