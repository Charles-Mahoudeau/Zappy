/*
** EPITECH PROJECT, 2026
** Server
** File description:
** Server code
*/

#include "zappy/server/net/Server.hpp"

#include <cstddef>
#include <cstdint>
#include <expected>
#include <iostream>
#include <string>
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
                          const auto result = this->makeNewConnection();
                          if (result.has_value()) {
                              clientRegistery.makeNewClient(this->_sockets, result.value(), timer);
                          } else {
                              std::cerr << result.error() << "\n";
                          }
                      });
}

bool Server::poll(int timeout) { return this->_poller.poll(timeout); }

std::expected<network::Address, std::string> Server::makeNewConnection() {
    network::socket::Client newClientSocket = this->_servSocket.accept();
    network::Address addr = newClientSocket.address();
    const int fd = newClientSocket.fd();

    try {
        newClientSocket.send("WELCOME\n");
    } catch (const zappy::exception::SocketError& err) {
        return std::unexpected("Failed to send WELCOME to " + addr.string() + ": " + err.what());
    }

    this->_sockets.insert(newClientSocket);

    this->_poller.add(fd, zappy::io::Poller::kPollRead, [this, addr](std::byte event) {
        auto* cli = this->_sockets.findByAddress(addr);
        bool fail = false;

        if (cli == nullptr) {
            std::cerr << "failed to find client for " + addr.string() + "\n";
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

    return addr;
}

}  // namespace zappy::server::net
