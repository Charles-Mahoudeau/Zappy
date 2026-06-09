/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** IServerSocket
*/

#pragma once

#include <sys/socket.h>

#include <cstdint>
#include <memory>

#include "zappy/shared/network/ISocket.hpp"

namespace zappy::network::socket {
class IClient;

class IServer : public virtual ISocket {
  public:
    IServer() = default;
    IServer(const IServer&) = delete;
    IServer(IServer&&) = default;
    ~IServer() override = default;

    IServer& operator=(const IServer&) = default;
    IServer& operator=(IServer&&) = default;

    virtual void listen(std::uint16_t maxConnections = SOMAXCONN) = 0;
    [[nodiscard]] virtual std::unique_ptr<IClient> accept() const = 0;
};
}  // namespace zappy::network::socket
