/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Logger
*/

#pragma once

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>

namespace zappy::io {
enum class Level : std::uint8_t { kDebug, kInfo, kWarning, kError, kFatal };

class Logger {
  public:
    Logger(std::string_view name, const std::filesystem::path& logFile, bool duplicateToStdout = false);
    Logger(std::string_view name);
    ~Logger() = default;

    Logger(const Logger&) = default;
    Logger& operator=(const Logger&) = default;

    Logger(Logger&&) noexcept = default;
    Logger& operator=(Logger&&) noexcept = default;

    [[nodiscard]] Logger derive(std::string_view name) const;
    void setLevel(Level level) noexcept;

    void log(Level level, std::string_view message) const;
    void debug(std::string_view message) const;
    void info(std::string_view message) const;
    void warning(std::string_view message) const;
    void error(std::string_view message) const;
    void fatal(std::string_view message) const;

  private:
    Logger(std::string prefix, std::shared_ptr<std::ofstream> file, bool duplicateToStdout);

    static std::string levelToString(Level level);
    static std::string timestamp();

    std::string _prefix;
    std::shared_ptr<std::ofstream> _file;
    bool _duplicateToStdout;
    Level _level{Level::kDebug};
};
}  // namespace zappy::io
