/*
** EPITECH PROJECT, 2026
** CliParser
** File description:
** CliParser code
*/

#include "zappy/server/CliParser.hpp"

#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <limits>
#include <map>
#include <optional>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "zappy/shared/exception/InvalidArgument.hpp"
namespace zappy::server {

CliParser::CliParser(std::span<std::string_view> argv) {
    CliParser::parseArguments(argv);
    CliParser::ensureValidArguments();
}

const CliParser::CliParameters& CliParser::parameters() { return this->_parameters; }

void CliParser::parseArguments(std::span<std::string_view> argv) {
    auto it = std::cbegin(argv);

    while (it != std::cend(argv)) {
        std::string_view param = *it;

        if (!param.starts_with("-")) {
            throw zappy::exception::InvalidArgument(std::format("invalid Cli argument: {}", param));
        }
        ++it;

        std::vector<std::string_view> flagParameters;
        while (it != std::cend(argv) && !it->starts_with("-")) {
            flagParameters.emplace_back(*it);
            ++it;
        }

        handleFlag(param.substr(1), flagParameters);
    }
}

template <typename T>
T CliParser::stringToUnNumber(std::string_view value, std::string_view flagName) {
    static constexpr unsigned long long maxVal = std::numeric_limits<T>::max();
    std::uint64_t parsed = 0;

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

void CliParser::handleFlag(std::string_view flag, const std::vector<std::string_view>& flagParams) {
    struct FlagBehavior {
        std::optional<size_t> nbParam = 1;
        std::function<void(const std::vector<std::string_view>& flagParams, CliParameters& param)> handle = nullptr;
    };
    static constexpr auto parseNb = []<typename T>(T& field, std::string_view flag,
                                                   const std::vector<std::string_view>& flagParams) {
        field = CliParser::stringToUnNumber<T>(flagParams.at(0), flag);
    };
    static const std::map<std::string_view, FlagBehavior> flags = {
        {"p", {.handle = [](const auto& flagParams, auto& param) { parseNb(param.port, "p", flagParams); }}},
        {"x", {.handle = [](const auto& flagParams, auto& param) { parseNb(param.mapWidth, "x", flagParams); }}},
        {"y", {.handle = [](const auto& flagParams, auto& param) { parseNb(param.mapHeight, "y", flagParams); }}},
        {"c", {.handle = [](const auto& flagParams, auto& param) { parseNb(param.nbInitialClient, "c", flagParams); }}},
        {"f", {.handle = [](const auto& flagParams, auto& param) { parseNb(param.frequencies, "f", flagParams); }}},
        {"n",
         {.nbParam = std::nullopt,
          .handle = [](const auto& flagParams, auto& param) { param.teamsName = flagParams; }}},
    };

    auto it = flags.find(flag);
    if (it == flags.end()) {
        throw exception::InvalidArgument(std::format("Unknown flag: {}", flag));
    }
    if (it->second.nbParam.has_value() && it->second.nbParam.value() != flagParams.size()) {
        throw exception::InvalidArgument(std::format("Invalid number parameter of {}", flag));
    }
    it->second.handle(flagParams, this->_parameters);
}

void CliParser::ensureValidArguments() const {
    const std::vector<std::pair<bool, std::string_view>> conditions = {
        {this->_parameters.port == 0, "port"},
        {this->_parameters.mapWidth < 10, "mapWidth"},
        {this->_parameters.mapHeight < 10, "mapHeight"},
        {this->_parameters.teamsName.empty(), "teamsName"},
        {this->_parameters.nbInitialClient == 0, "nbInitialClient"},
        {this->_parameters.frequencies > 10000, "frequencies"}};

    for (const auto& [failed, name] : conditions) {
        if (failed) {
            throw exception::InvalidArgument(std::format("Missing/Invalid required CLI argument: {}", name));
        }
    }
    for (const auto& teamName : this->_parameters.teamsName) {
        if (teamName.contains(" ")) {
            throw exception::InvalidArgument(std::format("Team space cannot contain spaces: {}", teamName));
        }
    }
}

}  // namespace zappy::server
