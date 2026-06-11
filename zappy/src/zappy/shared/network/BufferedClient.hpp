/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** BufferedClient
*/

#pragma once

#include <queue>
#include <string>
#include <string_view>

#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace zappy::network {
class BufferedClient {
  public:
    BufferedClient() = default;
    ~BufferedClient() = default;

    BufferedClient(const BufferedClient&) = delete;
    BufferedClient& operator=(const BufferedClient&) = delete;

    BufferedClient(BufferedClient&&) noexcept = default;
    BufferedClient& operator=(BufferedClient&&) noexcept = default;

    void connect(const Address& address) const;

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
