/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ClientSocket
*/

#pragma once

#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/Socket.hpp"

namespace zappy::network::socket {
class Client : public Socket {
  public:
    using Socket::Socket;

    void connect(const Address& address) const;
};
}  // namespace zappy::network::socket
