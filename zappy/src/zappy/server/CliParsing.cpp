/*
** EPITECH PROJECT, 2026
** CliParsing
** File description:
** CliParsing code
*/

#include "zappy/server/CliParsing.hpp"

#include <expected>
#include <format>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "zappy/shared/exception/InvalidArgument.hpp"
namespace zappy::server {

CliParsing::CliParameter CliParsing::parseArguments(const std::vector<std::string_view>& argv) {
    CliParameter params;

    for (size_t index = 0; index < argv.size(); ++index) {
        std::string_view param = argv[index];

        if (!param.starts_with("-")) {
            throw zappy::exception::InvalidArgument(std::format("invalid Cli argument: {}", param));
        }

        std::vector<std::string_view> flagParameters;
        while (index + 1 < argv.size() && !argv[index + 1].starts_with("-")) {
            ++index;
            flagParameters.emplace_back(argv[index]);
        }

        handleFlag(param.substr(1), flagParameters, params);
    }
    return params;
}

void CliParsing::handleFlag(std::string_view flag, const std::vector<std::string_view>& flagParams,
                            CliParameter& param) {
    struct FlagBehavior {
        std::optional<size_t> nbParam = 1;
        std::function<void()> handle = nullptr;
    };
    static const std::map<std::string_view, FlagBehavior> flags = {
        {"p", {.handle = [&flagParams, &param] { param.port = std::stoi(std::string(flagParams.at(0))); }}},
        {"x", {.handle = [&flagParams, &param] { param.mapWidth = std::stoul(std::string(flagParams.at(0))); }}},
        {"y", {.handle = [&flagParams, &param] { param.mapHeight = std::stoul(std::string(flagParams.at(0))); }}},
        {"n", {.nbParam = std::nullopt, .handle = [&flagParams, &param] { param.teamsName = flagParams; }}},
        {"c", {.handle = [&flagParams, &param] { param.nbInitialClient = std::stoul(std::string(flagParams.at(0))); }}},
        {"f", {.handle = [&flagParams, &param] { param.frequencies = std::stoul(std::string(flagParams.at(0))); }}},
    };

    auto it = flags.find(flag);
    if (it == flags.end()) {
        throw exception::InvalidArgument(std::format("Unknown flag: {}", flag));
    }
    if (it->second.nbParam.has_value() && it->second.nbParam.value() != flagParams.size()) {
        throw exception::InvalidArgument(std::format("Invalid number parameter of {}", flag));
    }
    it->second.handle();
}

void CliParsing::checkArgumentsValidity(const CliParameter& arguments) {
    static const std::vector<std::pair<bool, std::string_view>> conditions = {
        {arguments.port == 0, "port"},
        {arguments.mapWidth == 0, "mapWidth"},
        {arguments.mapHeight == 0, "mapHeight"},
        {arguments.teamsName.empty(), "teamsName"},
        {arguments.nbInitialClient == 0, "nbInitialClient"},
        {arguments.frequencies == 0, "frequencies"}};

    for (const auto& [failed, name] : conditions) {
        if (failed) {
            throw exception::InvalidArgument(std::format("Missing/Invalid required CLI argument: {}", name));
        }
    }
}

}  // namespace zappy::server
