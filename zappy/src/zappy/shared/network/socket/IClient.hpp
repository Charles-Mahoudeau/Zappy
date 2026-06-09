/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** IClientSocket
*/

#pragma once

#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/ISocket.hpp"

namespace zappy::network::socket {
class IClient : public virtual ISocket {
  public:
    IClient() = default;
    IClient(const IClient&) = delete;
    IClient(IClient&&) = default;
    ~IClient() override = default;

    IClient& operator=(const IClient&) = default;
    IClient& operator=(IClient&&) = default;

    virtual void connect(const Address& address) = 0;
};
}  // namespace zappy::network::socket
