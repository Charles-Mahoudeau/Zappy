/*
** EPITECH PROJECT, 2026
** Server
** File description:
** Server code
*/

#include "zappy/server/net/Server.hpp"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/io/Poller.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace zappy::server::net {

void Server::init(std::uint16_t port, client::ClientRegistry& clientRegistery, Timer& timer) {
    this->_servSocket.bind(port);
    this->_servSocket.listen();

    this->_poller.add(this->_servSocket, zappy::io::Poller::kPollRead,
                      [this, &clientRegistery, &timer](std::byte /*event*/) {
                          const network::Address addr = this->makeNewConnection();
                          clientRegistery.makeNewClient(this->_sockets, addr, timer);
                      });
}

bool Server::poll(int timeout) { return this->_poller.poll(timeout); }

network::Address Server::makeNewConnection() {
    network::socket::Client newClientSocket = this->_servSocket.accept();
    network::Address addr = newClientSocket.address();
    (void)newClientSocket.send("WELCOME\n");

    this->_poller.add(newClientSocket.fd(), zappy::io::Poller::kPollRead, [this, addr](std::byte event) {
        auto* cli = this->_sockets.findByAddress(addr);
        bool fail = false;

        if (cli == nullptr) {
            std::cerr << "failed to find client\n";
            return;
        }

        if ((event & zappy::io::Poller::kPollRead) != std::byte{0}) {
            try {
                cli->poll();
            } catch (const zappy::exception::SocketError&) {
                fail = true;
            }
        }
        if ((event & zappy::io::Poller::kPollError) != std::byte{0} || fail) {
            this->_sockets.remove(cli->addr());
            this->_poller.remove(cli->fd());
        }
    });

    this->_sockets.insert(newClientSocket);
    return addr;
}

}  // namespace zappy::server::net
