/*
** EPITECH PROJECT, 2026
** CliParsing
** File description:
** CliParsing header
*/

#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

namespace zappy::server {
class CliParsing {
  public:
    struct CliParameter {
        std::uint16_t port = 0;
        std::uint32_t mapWidth = 0;
        std::uint32_t mapHeight = 0;
        std::vector<std::string_view> teamsName;
        std::uint32_t nbInitialClient = 0;
        std::uint32_t frequencies = 0;
    };

    static CliParameter parseArguments(const std::vector<std::string_view>& argv);
    static void checkArgumentsValidity(const CliParameter& arguments);

  private:
    static constexpr std::string_view OVERFLOW_MESSAGE = "Value overflow for -{}: '{}' exceeds allowed range [0, {}]";

    static void handleFlag(std::string_view flag, const std::vector<std::string_view>& flagParams, CliParameter& param);

    template <typename T>
    static T parseAndValidate(std::string_view value, std::string_view flagName);
};

}  // namespace zappy::server
