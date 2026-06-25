/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Logger
*/

#pragma once

#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <memory>
#include <string>
#include <string_view>

namespace zappy::io {
enum class Level : std::uint8_t { kDebug, kInfo, kWarning, kError, kFatal };

class Logger {
  public:
    Logger(std::string_view name, const std::filesystem::path& logFile, bool duplicateToStdout = false);
    explicit Logger(std::string_view name);
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
    void warn(std::string_view message) const;
    void error(std::string_view message) const;
    void fatal(std::string_view message) const;

    template <typename... Args>
    void log(Level level, std::format_string<Args...> fmt, Args&&... args) const;
    template <typename... Args>
    void debug(std::format_string<Args...> fmt, Args&&... args) const;
    template <typename... Args>
    void info(std::format_string<Args...> fmt, Args&&... args) const;
    template <typename... Args>
    void warn(std::format_string<Args...> fmt, Args&&... args) const;
    template <typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args) const;
    template <typename... Args>
    void fatal(std::format_string<Args...> fmt, Args&&... args) const;

  private:
    Logger(std::string prefix, std::shared_ptr<std::ofstream> file, bool duplicateToStdout);

    static std::string levelToString(Level level);
    static std::string timestamp();

    std::string _prefix;
    std::shared_ptr<std::ofstream> _file;
    bool _duplicateToStdout;
    Level _level{Level::kDebug};
};

template <typename... Args>
void Logger::log(Level level, std::format_string<Args...> fmt, Args&&... args) const {
    this->log(level, std::format(fmt, std::forward<Args>(args)...));
}
template <typename... Args>
void Logger::debug(std::format_string<Args...> fmt, Args&&... args) const {
    this->debug(std::format(fmt, std::forward<Args>(args)...));
}
template <typename... Args>
void Logger::info(std::format_string<Args...> fmt, Args&&... args) const {
    this->info(std::format(fmt, std::forward<Args>(args)...));
}
template <typename... Args>
void Logger::warn(std::format_string<Args...> fmt, Args&&... args) const {
    this->warn(std::format(fmt, std::forward<Args>(args)...));
}
template <typename... Args>
void Logger::error(std::format_string<Args...> fmt, Args&&... args) const {
    this->error(std::format(fmt, std::forward<Args>(args)...));
}
template <typename... Args>
void Logger::fatal(std::format_string<Args...> fmt, Args&&... args) const {
    this->fatal(std::format(fmt, std::forward<Args>(args)...));
}
}  // namespace zappy::io
