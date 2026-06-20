/*
** EPITECH PROJECT, 2026
** SocketRegistery
** File description:
** SocketRegistery code
*/

#include "zappy/server/net/SocketRegistry.hpp"

#include <utility>

#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/BufferedClient.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace zappy::server::net {

void SocketRegistry::insert(zappy::network::socket::Client& socket) {
    network::BufferedClient client(socket);

    this->_sockets.emplace_back(std::move(client));
}

network::BufferedClient* SocketRegistry::findByAddress(const network::Address& addr) {
    for (auto& socket : this->_sockets) {
        if (socket == addr) {
            return &socket;
        }
    }
    return nullptr;
}

void SocketRegistry::remove(const network::Address& addr) {
    std::erase_if(this->_sockets, [&addr](const network::BufferedClient& socket) { return addr == socket; });
}

void SocketRegistry::clear() { this->_sockets.clear(); }

int SocketRegistry::size() { return this->_sockets.size(); }

}  // namespace zappy::server::net
