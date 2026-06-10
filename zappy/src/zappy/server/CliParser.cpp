/*
** EPITECH PROJECT, 2026
** CliParser
** File description:
** CliParser code
*/

#include "zappy/server/CliParser.hpp"

#include <cstddef>
#include <format>
#include <functional>
#include <limits>
#include <map>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "zappy/shared/exception/InvalidArgument.hpp"
namespace zappy::server {

CliParser::CliParameter CliParser::parseArguments(const std::vector<std::string_view>& argv) {
    CliParameter params;
    auto it = argv.cbegin();

    while (it != argv.cend()) {
        std::string_view param = *it;

        if (!param.starts_with("-")) {
            throw zappy::exception::InvalidArgument(std::format("invalid Cli argument: {}", param));
        }
        ++it;

        std::vector<std::string_view> flagParameters;
        while (it != argv.cend() && !it->starts_with("-")) {
            flagParameters.emplace_back(*it);
            ++it;
        }

        handleFlag(param.substr(1), flagParameters, params);
    }
    return params;
}

template <typename T>
T CliParser::parseAndValidate(std::string_view value, std::string_view flagName) {
    static constexpr unsigned long long maxVal = std::numeric_limits<T>::max();

    unsigned long long parsed = 0;

    if (!value.empty() && value.at(0) == '-') {
        throw exception::InvalidArgument(std::format(OVERFLOW_MESSAGE, flagName, value, maxVal));
    }
    try {
        size_t pos = 0;
        parsed = std::stoull(std::string(value), &pos);
        if (pos != value.size()) {
            throw exception::InvalidArgument(
                std::format("Invalid value for -{}: '{}' is not a valid number", flagName, value));
        }
    } catch (const std::invalid_argument&) {
        throw exception::InvalidArgument(
            std::format("Invalid value for -{}: '{}' is not a valid number", flagName, value));
    } catch (const std::out_of_range&) {
        throw exception::InvalidArgument(std::format(OVERFLOW_MESSAGE, flagName, value, maxVal));
    }

    if (parsed > maxVal) {
        throw exception::InvalidArgument(std::format(OVERFLOW_MESSAGE, flagName, value, maxVal));
    }
    return static_cast<T>(parsed);
}

void CliParser::handleFlag(std::string_view flag, const std::vector<std::string_view>& flagParams,
                           CliParameter& param) {
    struct FlagBehavior {
        std::optional<size_t> nbParam = 1;
        std::function<void()> handle = nullptr;
    };
    auto parseNb = [&flagParams]<typename T>(T& field, std::string_view flag) {
        field = parseAndValidate<T>(flagParams.at(0), flag);
    };
    const std::map<std::string_view, FlagBehavior> flags = {
        {"p", {.handle = [&param, &parseNb] { parseNb(param.port, "p"); }}},
        {"x", {.handle = [&param, &parseNb] { parseNb(param.mapWidth, "x"); }}},
        {"y", {.handle = [&param, &parseNb] { parseNb(param.mapHeight, "y"); }}},
        {"c", {.handle = [&param, &parseNb] { parseNb(param.nbInitialClient, "c"); }}},
        {"f", {.handle = [&param, &parseNb] { parseNb(param.frequencies, "f"); }}},
        {"n", {.nbParam = std::nullopt, .handle = [&param, &flagParams] { param.teamsName = flagParams; }}},
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

void CliParser::checkArgumentsValidity(const CliParameter& arguments) {
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
