/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** CommandSender
*/

#pragma once

#include <cstdint>
#include <functional>

#include "zappy/shared/network/BufferedClient.hpp"

namespace zappy::gui::network {

class CommandSender {
  public:
    explicit CommandSender(zappy::network::BufferedClient& buffer);
    ~CommandSender() = default;

    CommandSender(const CommandSender&) = delete;
    CommandSender& operator=(const CommandSender&) = delete;

    CommandSender(CommandSender&&) noexcept = default;
    CommandSender& operator=(CommandSender&&) noexcept = default;

    void requestPlayer(std::uint32_t id);
    void requestTile(std::uint32_t x, std::uint32_t y);
    void setTimeUnit(int t);
    void requestInitialState();

  private:
    std::reference_wrapper<zappy::network::BufferedClient> _buffer;
};

}  // namespace zappy::gui::network
