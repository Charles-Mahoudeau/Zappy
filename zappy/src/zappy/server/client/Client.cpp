/*
** EPITECH PROJECT, 2026
** Client
** File description:
** Client code
*/

#include "zappy/server/client/Client.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/BufferedClient.hpp"

namespace zappy::server {

Client::Client(net::SocketRegistry& socketRegister, network::Address address)
    : _addr(address), _socketsRegistery(socketRegister) {}

const network::Address& Client::address() const { return this->_addr; }

Client::Type Client::type() const { return this->_type; }

void Client::changeType(Client::Type type) { this->_type = type; }

void Client::addRequest(std::string msg) {
    if (this->_requests.size() >= kMaxRequests) {
        return;
    }
    this->_requests.emplace(std::move(msg));
}

bool Client::update() {
    auto result = this->_socketsRegistery.getFromAddress(this->_addr);
    if (!result.has_value()) {
        return false;
    }
    network::BufferedClient& socket = result.value().get();

    while (socket.hasMessages()) {
        std::string msg = socket.popMessage();

        this->addRequest(std::move(msg));
    }
    return true;
}

std::optional<std::string> Client::nextRequest() {
    if (this->_timeout > 0) {
        this->_timeout--;
        return std::nullopt;
    }
    if (this->_requests.empty()) {
        return std::nullopt;
    }
    std::string request = this->_requests.front();
    this->_requests.pop();
    return request;
}

void Client::setTimeout(int timeout) { this->_timeout = timeout; }

bool Client::sendMessage(std::string_view msg) {
    auto socket = this->_socketsRegistery.getFromAddress(this->_addr);
    if (!socket.has_value()) {
        return false;
    }
    try {
        socket.value().get().send(msg);
    } catch (const zappy::exception::SocketError& /*err */) {
        return false;
    }
    return true;
}

}  // namespace zappy::server
