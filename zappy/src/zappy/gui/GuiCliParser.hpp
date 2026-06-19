/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GuiCliParser
*/

#pragma once

#include <cstdint>
#include <span>
#include <string_view>

namespace zappy::gui {

class GuiCliParser {
  public:
    explicit GuiCliParser(std::span<const std::string_view> args);
    ~GuiCliParser() = default;

    GuiCliParser(const GuiCliParser&) = delete;
    GuiCliParser& operator=(const GuiCliParser&) = delete;

    GuiCliParser(GuiCliParser&&) noexcept = default;
    GuiCliParser& operator=(GuiCliParser&&) noexcept = default;

    [[nodiscard]] std::string_view host() const;
    [[nodiscard]] std::uint16_t port() const;

  private:
    std::string_view _host;
    std::uint16_t _port{0};
};

}  // namespace zappy::gui
