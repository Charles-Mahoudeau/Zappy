/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Logger
*/

#include "Logger.hpp"

#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "zappy/shared/exception/InvalidState.hpp"

namespace zappy::io {
Logger::Logger(std::string_view name, const std::filesystem::path& logFile, const bool duplicateToStdout)
    : _prefix{std::format("[{}]", name)},
      _file{std::make_shared<std::ofstream>(logFile, std::ios::app)},
      _duplicateToStdout{duplicateToStdout} {
    if (!_file->is_open()) {
        throw exception::InvalidState{"cannot open log file: " + logFile.string()};
    }
}

Logger::Logger(std::string_view name) : _prefix{std::format("[{}]", name)}, _file{nullptr}, _duplicateToStdout{true} {}

Logger Logger::derive(std::string_view name) const {
    Logger logger{std::format("{} [{}]", _prefix, name), _file, _duplicateToStdout};

    logger.setLevel(_level);
    return logger;
}

void Logger::setLevel(const Level level) noexcept { _level = level; }

void Logger::log(const Level level, std::string_view message) const {
    if (std::to_underlying(level) < std::to_underlying(_level)) {
        return;
    }

    const std::string line = std::format("[{}] [{}] {} {}\n", timestamp(), levelToString(level), _prefix, message);

    if (_file) {
        *_file << line;
        _file->flush();
    }
    if (_duplicateToStdout) {
        std::cout << line;
    }
}

void Logger::debug(const std::string_view message) const { log(Level::kDebug, message); }

void Logger::info(const std::string_view message) const { log(Level::kInfo, message); }

void Logger::warn(const std::string_view message) const { log(Level::kWarn, message); }

void Logger::error(const std::string_view message) const { log(Level::kError, message); }

void Logger::fatal(const std::string_view message) const { log(Level::kFatal, message); }

Logger::Logger(std::string prefix, std::shared_ptr<std::ofstream> file, const bool duplicateToStdout)
    : _prefix{std::move(prefix)}, _file{std::move(file)}, _duplicateToStdout{duplicateToStdout} {}

std::string Logger::levelToString(const Level level) {
    switch (level) {
        using enum Level;

        case kDebug:
            return "DEBUG";
        case kInfo:
            return "INFO";
        case kWarn:
            return "WARN";
        case kError:
            return "ERROR";
        case kFatal:
            return "FATAL";
    }
    return "UNKNOWN";
}
std::string Logger::timestamp() {
    const auto now = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());

    return std::format("{:%Y-%m-%d %H:%M:%S}", now);
}
}  // namespace zappy::io
