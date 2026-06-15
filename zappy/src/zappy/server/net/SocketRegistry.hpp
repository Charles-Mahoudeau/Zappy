/*
** EPITECH PROJECT, 2026
** SocketRegistery
** File description:
** SocketRegistery header
*/

#pragma once

#include <expected>
#include <functional>
#include <string>
#include <vector>

#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/BufferedClient.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace zappy::server::net {

class SocketRegistry {
  public:
    SocketRegistry() = default;
    ~SocketRegistry() = default;

    SocketRegistry(const SocketRegistry&) = delete;
    SocketRegistry(SocketRegistry&&) = delete;
    SocketRegistry& operator=(const SocketRegistry&) = delete;
    SocketRegistry& operator=(SocketRegistry&&) = delete;

    void insert(zappy::network::socket::Client& socket);

    void remove(int fd);

    void clear();

    std::expected<std::reference_wrapper<network::BufferedClient>, std::string> getFromAddress(network::Address& addr);

  private:
    std::vector<zappy::network::BufferedClient> _sockets;
    // Members
};

}  // namespace zappy::server::net
