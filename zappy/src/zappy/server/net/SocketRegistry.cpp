/*
** EPITECH PROJECT, 2026
** SocketRegistery
** File description:
** SocketRegistery code
*/

#include "zappy/server/net/SocketRegistry.hpp"

#include <expected>
#include <functional>
#include <string>
#include <utility>

#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/BufferedClient.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace zappy::server::net {

void SocketRegistry::insert(zappy::network::socket::Client& socket) {
    network::BufferedClient client(socket);

    this->_sockets.emplace_back(std::move(client));
}

std::expected<std::reference_wrapper<network::BufferedClient>, std::string> SocketRegistry::getFromAddress(
    network::Address& addr) {
    for (auto& socket : this->_sockets) {
        if (socket == addr) {
            return {socket};
        }
    }
    return std::unexpected("Failed to get client Socket");
}

void SocketRegistry::remove(int fd) {
    std::erase_if(this->_sockets, [fd](const network::BufferedClient& socket) { return fd == socket.fd(); });
}

}  // namespace zappy::server::net
