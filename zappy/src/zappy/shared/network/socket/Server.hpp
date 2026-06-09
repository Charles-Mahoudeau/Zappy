/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ServerSocket
*/

#pragma once

#include <sys/socket.h>

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

#include "Client.hpp"
#include "IServer.hpp"
#include "zappy/shared/network/Socket.hpp"

namespace zappy::network::socket {
class Server : public Socket, public IServer {
  public:
    using ConnectCallback = std::function<void(std::shared_ptr<Client>)>;
    using MessageCallback = std::function<void(std::shared_ptr<Client>, const std::string&)>;

    using Socket::Socket;

    void listen(std::uint16_t maxConnections = SOMAXCONN) override;
    [[nodiscard]] std::unique_ptr<IClient> accept() const override;

  private:
    std::uint16_t _maxConnections{0};
};
}  // namespace zappy::network::socket
