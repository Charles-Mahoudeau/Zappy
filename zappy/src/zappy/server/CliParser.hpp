/*
** EPITECH PROJECT, 2026
** CliParser
** File description:
** CliParser header
*/

#pragma once

#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

namespace zappy::server {
class CliParser {
  public:
    explicit CliParser(std::span<std::string_view> argv);
    ~CliParser() = default;

    CliParser(const CliParser&) = delete;
    CliParser(CliParser&&) = delete;
    CliParser& operator=(const CliParser&) = default;
    CliParser& operator=(CliParser&&) = delete;

    /**
     * @brief Structure holding all CLI configuration parameters.
     */
    struct CliParameters {
        std::uint16_t port = 0;
        std::uint32_t mapWidth = 0;
        std::uint32_t mapHeight = 0;
        std::vector<std::string_view> teamsName;
        std::uint32_t nbInitialClient = 0;
        std::uint32_t frequencies = 0;
    };

    const CliParameters& parameters();

  private:
    CliParameters _parameters;

    static constexpr std::string_view OVERFLOW_MESSAGE = "Value overflow for -{}: '{}' exceeds allowed range [0, {}]";

    /**
     * @brief Parses command-line arguments into CLI parameters.
     *
     * @param argv Vector of command-line argument strings.
     * @return CliParameter struct containing parsed configuration values.
     * @throws exception::InvalidArgument if an unknown flag is encountered, required parameters are missing, or values
     * are invalid.
     */
    static CliParameters parseArguments(std::span<std::string_view> argv);

    /**
     * @brief Validates that all required CLI arguments are present and valid.
     *
     * @param arguments The CLI parameter structure to validate.
     * @throws exception::InvalidArgument if any required argument is missing or invalid (port, mapWidth, mapHeight,
     * teamsName (at least 2), nbInitialClient, frequencies).
     */
    static void ensureValidArguments(const CliParameters& arguments);

    /**
     * @brief Handles a specific flag with its associated parameters.
     *
     * @param flag The flag name (without the leading dash).
     * @param flagParams Vector of parameters associated with this flag.
     * @param param The CLI parameter structure to populate.
     * @throws exception::InvalidArgument if the flag is unknown or has invalid number of parameters.
     */
    static void handleFlag(std::string_view flag, const std::vector<std::string_view>& flagParams,
                           CliParameters& param);

    /**
     * @brief Parses and validates a numeric string value.
     *
     * @tparam T Numeric type to parse the value into.
     * @param value The string value to parse.
     * @param flagName The name of the flag for error messages.
     * @return The parsed numeric value of type T.
     * @throws exception::InvalidArgument if the value is not a valid number, exceeds the type's max value, or is
     * negative.
     */
    template <typename T>
    static T stringToUnNumber(std::string_view value, std::string_view flagName);
};

}  // namespace zappy::server
