/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ClientSocket
*/

#pragma once

#include "IClient.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/Socket.hpp"

namespace zappy::network::socket {
class Client : public Socket, public IClient {
  public:
    using Socket::Socket;

    void connect(const Address& address) override;
};
}  // namespace zappy::network::socket
