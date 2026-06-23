/*
** EPITECH PROJECT, 2026
** SocketRegistery
** File description:
** SocketRegistery header
*/

#pragma once

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

    /**
     * @brief insert a socket to the Registry
     * @param socket A Client type Socket which will then stored as a BufferedClient
     */
    void insert(zappy::network::socket::Client& socket);

    /**
     * @brief remove a client from the registry
     * @param fd file descriptor of the socket
     */
    void remove(int fd);

    /**
     * @brief Remove all socket from the registry
     */
    void clear();

    /**
     * @brief use a address to get the socket related to it
     * @param addr address of the socket
     * @return ptr of the socket as BufferedClient. nullptr if fail to find it
     */
    network::BufferedClient* findByAddress(const network::Address& addr);

  private:
    std::vector<zappy::network::BufferedClient> _sockets;
    // Members
};

}  // namespace zappy::server::net
