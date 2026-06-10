/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** LineBuffer
*/

#pragma once

#include <queue>
#include <string>
#include <string_view>

#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace zappy::network {
class LineBuffer {
  public:
    LineBuffer() = default;
    ~LineBuffer() = default;

    LineBuffer(const LineBuffer&) = delete;
    LineBuffer& operator=(const LineBuffer&) = delete;

    LineBuffer(LineBuffer&&) noexcept = default;
    LineBuffer& operator=(LineBuffer&&) noexcept = default;

    void connect(const Address& address);

    [[nodiscard]] int fd() const;
    [[nodiscard]] bool hasMessages() const;
    [[nodiscard]] std::string popMessage();

    void poll();
    void send(std::string_view line);

  private:
    socket::Client _client;
    std::string _buffer{};
    std::queue<std::string> _messages{};
};
}  // namespace zappy::network
