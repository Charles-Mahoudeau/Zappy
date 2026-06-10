/*
** EPITECH PROJECT, 2026
** CliParsing
** File description:
** CliParsing code
*/

#include "zappy/server/CliParsing.hpp"

#include <format>
#include <functional>
#include <map>
#include <stdexcept>
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

template <typename T>
T CliParsing::parseAndValidate(std::string_view value, std::string_view flagName) {
    static constexpr unsigned long long maxVal = std::numeric_limits<T>::max();

    unsigned long long parsed = 0;

    if (!value.empty() && value[0] == '-') {
        throw exception::InvalidArgument(std::format(OVERFLOW_MESSAGE, flagName, value, maxVal));
    }
    try {
        size_t pos = 0;
        if (parsed = std::stoull(std::string(value), &pos); pos != value.size()) {
            throw exception::InvalidArgument(
                std::format("Invalid value for -{}: '{}' is not a valid number", flagName, value));
        }
    } catch (const std::logic_error&) {
        throw exception::InvalidArgument(std::format(OVERFLOW_MESSAGE, flagName, value, maxVal));
    }

    if (parsed > maxVal) {
        throw exception::InvalidArgument(std::format(OVERFLOW_MESSAGE, flagName, value, maxVal));
    }
    return static_cast<T>(parsed);
}

void CliParsing::handleFlag(std::string_view flag, const std::vector<std::string_view>& flagParams,
                            CliParameter& param) {
    struct FlagBehavior {
        std::optional<size_t> nbParam = 1;
        std::function<void()> handle = nullptr;
    };
    auto parseNb = [&flagParams](auto& field, std::string_view flag) {
        field = parseAndValidate<std::decay_t<decltype(field)>>(flagParams.at(0), flag);
    };
    static const std::map<std::string_view, FlagBehavior> flags = {
        {"p", {.handle = [&] { parseNb(param.port, "p"); }}},
        {"x", {.handle = [&] { parseNb(param.mapWidth, "x"); }}},
        {"y", {.handle = [&] { parseNb(param.mapHeight, "y"); }}},
        {"c", {.handle = [&] { parseNb(param.nbInitialClient, "c"); }}},
        {"f", {.handle = [&] { parseNb(param.frequencies, "f"); }}},
        {"n", {.nbParam = std::nullopt, .handle = [&] { param.teamsName = flagParams; }}},
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
        {arguments.teamsName.size() < 2, "teamsName"},
        {arguments.nbInitialClient == 0, "nbInitialClient"},
        {arguments.frequencies == 0, "frequencies"}};

    for (const auto& [failed, name] : conditions) {
        if (failed) {
            throw exception::InvalidArgument(std::format("Missing/Invalid required CLI argument: {}", name));
        }
    }
}

}  // namespace zappy::server
