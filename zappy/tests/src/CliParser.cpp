/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Cli Parsing Test
*/

#include "zappy/server/CliParser.hpp"

#include <gtest/gtest.h>

#include <span>
#include <string_view>
#include <vector>

#include "zappy/shared/exception/InvalidArgument.hpp"

using Params = zappy::server::CliParser::CliParameters;
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

// Builds a CliParser from argv (parsing + validation happen in the constructor).
CLI build(std::vector<std::string_view> argv) { return CLI{std::span<std::string_view>(argv)}; }

}  // namespace

// ─────────────────────────────────────────────
//  Happy path
// ─────────────────────────────────────────────

TEST(CliParser, valid_full_arguments_parsed_correctly) {
    auto argv = makeFullArgv();
    CLI parser{std::span<std::string_view>(argv)};
    const auto& params = parser.parameters();

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
    std::vector<std::string_view> argv = {"-f", "50", "-c", "3",  "-n", "red", "blue",
                                          "-y", "15", "-x", "25", "-p", "1234"};
    CLI parser{std::span<std::string_view>(argv)};
    const auto& params = parser.parameters();

    EXPECT_EQ(params.port, 1234);
    EXPECT_EQ(params.mapWidth, 25U);
    EXPECT_EQ(params.mapHeight, 15U);
    EXPECT_EQ(params.frequencies, 50U);
    EXPECT_EQ(params.nbInitialClient, 3U);
    ASSERT_EQ(params.teamsName.size(), 2U);
}

TEST(CliParser, empty_argv_throws) {
    const std::vector<std::string_view> argv = {};
    ASSERT_THROW(build(argv), InvalidArg);
}

// ─────────────────────────────────────────────
//  Malformed flags
// ─────────────────────────────────────────────

TEST(CliParser, unknown_flag_throws) { ASSERT_THROW(build({"-z", "42"}), InvalidArg); }

TEST(CliParser, no_leading_dash_throws) { ASSERT_THROW(build({"p", "4242"}), InvalidArg); }

TEST(CliParser, double_dash_flag_throws) { ASSERT_THROW(build({"--port", "4242"}), InvalidArg); }

TEST(CliParser, only_dash_flag_throws) { ASSERT_THROW(build({"-", "4242"}), InvalidArg); }

// ─────────────────────────────────────────────
//  Parameter count
// ─────────────────────────────────────────────

TEST(CliParser, flag_without_required_parameter_throws) { ASSERT_THROW(build({"-p"}), InvalidArg); }

TEST(CliParser, flag_with_too_many_parameters_throws) { ASSERT_THROW(build({"-p", "4242", "9999"}), InvalidArg); }

// ─────────────────────────────────────────────
//  Value parsing errors
// ─────────────────────────────────────────────

TEST(CliParser, non_numeric_value_throws) { ASSERT_THROW(build({"-p", "abc"}), InvalidArg); }

TEST(CliParser, empty_value_throws) { ASSERT_THROW(build({"-p", ""}), InvalidArg); }

TEST(CliParser, partial_numeric_value_throws) { ASSERT_THROW(build({"-p", "42abc"}), InvalidArg); }

TEST(CliParser, negative_value_throws) { ASSERT_THROW(build({"-p", "-1"}), InvalidArg); }

// ─────────────────────────────────────────────
//  Overflow handling
// ─────────────────────────────────────────────

TEST(CliParser, uint16_field_above_max_throws) { ASSERT_THROW(build({"-p", "65536"}), InvalidArg); }

TEST(CliParser, uint16_field_at_max_accepted) {
    // 65535 is a valid port, but the other required args are missing,
    // so the constructor still rejects it during validation.
    // Build a fully-valid argv with port at max to assert the value is parsed.
    auto argv = makeFullArgv("65535");
    CLI parser{std::span<std::string_view>(argv)};
    EXPECT_EQ(parser.parameters().port, 65535);
}

TEST(CliParser, uint32_field_above_max_throws) {
    ASSERT_THROW(build({"-x", "99999999999999999999999999"}), InvalidArg);
}

// ─────────────────────────────────────────────
//  Validity (now enforced by the constructor)
// ─────────────────────────────────────────────

TEST(CliParser, validity_rejects_zero_port) {
    auto argv = makeFullArgv("0");
    ASSERT_THROW(build(argv), InvalidArg);
}

TEST(CliParser, validity_rejects_zero_width) {
    auto argv = makeFullArgv("4242", "0");
    ASSERT_THROW(build(argv), InvalidArg);
}

TEST(CliParser, validity_rejects_zero_height) {
    auto argv = makeFullArgv("4242", "10", "0");
    ASSERT_THROW(build(argv), InvalidArg);
}

TEST(CliParser, validity_rejects_zero_clients) {
    auto argv = makeFullArgv("4242", "10", "10", {"teamA", "teamB"}, "0");
    ASSERT_THROW(build(argv), InvalidArg);
}

TEST(CliParser, validity_rejects_to_high_frequency) {
    auto argv = makeFullArgv("4242", "10", "10", {"teamA", "teamB"}, "5", "10001");
    ASSERT_THROW(build(argv), InvalidArg);
}

TEST(CliParser, validity_rejects_empty_teams) {
    auto argv = makeFullArgv("4242", "10", "10", std::vector<std::string_view>{});
    ASSERT_THROW(build(argv), InvalidArg);
}

TEST(CliParser, validity_reject_width_below_10) {
    auto argv = makeFullArgv("4242", "5", "10");
    ASSERT_THROW(build(argv), InvalidArg);
}

TEST(CliParser, validity_reject_height_below_10) {
    auto argv = makeFullArgv("4242", "10", "5");
    ASSERT_THROW(build(argv), InvalidArg);
}
