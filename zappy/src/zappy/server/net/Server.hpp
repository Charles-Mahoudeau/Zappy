/*
** EPITECH PROJECT, 2026
** Server
** File description:
** Server header
*/

#pragma once

#include <cstdint>

#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/io/Poller.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/socket/Server.hpp"

namespace zappy::server::net {
class Server {
  public:
    Server() = default;
    ~Server() = default;

    Server(const Server&) = delete;
    Server(Server&&) = delete;
    Server& operator=(const Server&) = delete;
    Server& operator=(Server&&) = delete;

    void init(std::uint16_t port, std::uint8_t freq, client::ClientRegistry& clientRegistery);
    bool update();

  private:
    std::uint8_t _freq = -1;
    zappy::network::socket::Server _servSocket{};
    SocketRegistry _sockets;
    io::Poller _poller;

    network::Address makeNewConnection();
};

}  // namespace zappy::server::net
