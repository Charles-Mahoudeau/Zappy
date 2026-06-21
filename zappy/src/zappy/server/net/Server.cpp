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

#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/io/Poller.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace zappy::server::net {

void Server::init(std::uint16_t port, std::uint8_t freq, client::ClientRegistry& clientRegistery) {
    this->_servSocket.bind(port);
    this->_servSocket.listen();
    this->_freq = freq;

    this->_poller.add(this->_servSocket, zappy::io::Poller::kPollRead, [this, &clientRegistery](std::byte /*event*/) {
        const network::Address addr = this->makeNewConnection();
        clientRegistery.makeNewClient(this->_sockets, addr);
        std::cout << addr << "\n";
    });
}

bool Server::update() {
    this->_poller.poll(this->_freq);
    return true;
}

network::Address Server::makeNewConnection() {
    network::socket::Client newClientSocket = this->_servSocket.accept();
    network::Address const addr = newClientSocket.address();
    (void)newClientSocket.send("WELCOME\n");

    this->_sockets.insert(newClientSocket);
    auto* cli = this->_sockets.findByAddress(addr);

    this->_poller.add(cli->fd(), zappy::io::Poller::kPollRead, [this, cli](std::byte event) {
        bool fail = false;
        const int fd = cli->fd();

        if ((event & zappy::io::Poller::kPollRead) != std::byte{0}) {
            try {
                cli->poll();
            } catch (const zappy::exception::SocketError&) {
                fail = true;
            }
        }
        if ((event & zappy::io::Poller::kPollError) != std::byte{0} || fail) {
            this->_sockets.remove(cli->addr());
            this->_poller.remove(fd);
        }
    });
    return addr;
}

}  // namespace zappy::server::net
