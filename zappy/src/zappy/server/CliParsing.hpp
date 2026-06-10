/*
** EPITECH PROJECT, 2026
** CliParsing
** File description:
** CliParsing header
*/

#pragma once

#include <cstdint>
#include <expected>
#include <string>
#include <string_view>
#include <vector>

namespace zappy::server {
class CliParsing {
  public:
    CliParsing() = default;
    ~CliParsing() = default;

    CliParsing(const CliParsing&) = default;
    CliParsing(CliParsing&&) = delete;
    CliParsing& operator=(const CliParsing&) = default;
    CliParsing& operator=(CliParsing&&) = delete;

    struct CliParameter {
        std::uint16_t port = 0;
        std::uint64_t mapWidth = 0;
        std::uint64_t mapHeight = 0;
        std::vector<std::string_view> teamsName;
        std::uint64_t nbInitialClient = 0;
        std::uint64_t frequencies = 0;
    };

    static CliParameter parseArguments(const std::vector<std::string_view>& argv);
    static std::expected<void, std::string> checkArgumentsValidity(const CliParameter& arguments);

  private:
    static void handleFlag(std::string_view flag, const std::vector<std::string_view>& flagParams, CliParameter& param);
};

}  // namespace zappy::server
