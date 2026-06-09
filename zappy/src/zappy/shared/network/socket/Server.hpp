/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ServerSocket
*/

#pragma once

#include <sys/socket.h>

#include <cstdint>

#include "Client.hpp"
#include "zappy/shared/network/Socket.hpp"

namespace zappy::network::socket {
class Server : public Socket {
  public:
    using Socket::Socket;

    void listen(std::uint16_t maxConnections = SOMAXCONN);
    [[nodiscard]] Client accept() const;

  private:
    static constexpr std::uint8_t kAcceptMaximumAttempts = 10;

    std::uint16_t _maxConnections{0};
};
}  // namespace zappy::network::socket
