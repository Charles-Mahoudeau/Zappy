/*
** EPITECH PROJECT, 2026
** Client
** File description:
** Client code
*/

#include "zappy/server/client/Client.hpp"

#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

#include "zappy/server/Timer.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/BufferedClient.hpp"

namespace zappy::server {

Client::Client(net::SocketRegistry& socketRegister, network::Address address, Timer& timer)
    : _timer(timer), _addr(address), _socketsRegistery(socketRegister) {}

Client::~Client() { this->removeTimeout(); }

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
    auto* socket = this->_socketsRegistery.findByAddress(this->_addr);

    if (socket == nullptr) {
        return false;
    }

    while (socket->hasMessages()) {
        std::string msg = socket->popMessage();

        this->addRequest(std::move(msg));
    }
    return true;
}

std::optional<std::string> Client::nextRequest() {
    if (this->inTimeout() || this->_requests.empty()) {
        return std::nullopt;
    }
    std::string request = this->_requests.front();
    this->_requests.pop();
    return request;
}

bool Client::setTimeout(int time) {
    if (this->inTimeout() || time <= 0) {
        return false;
    }

    this->_timeoutId = this->_timer.scheduleLater(time, [this]() {
        std::cout << "finish busy\n";
        this->_timeoutId = 0;
    });
    return true;
}

void Client::removeTimeout() {
    this->_timer.unschedule(this->_timeoutId);
    this->_timeoutId = 0;
}

bool Client::sendMessage(std::string_view msg) const {
    auto* socket = this->_socketsRegistery.findByAddress(this->_addr);

    if (socket == nullptr) {
        return false;
    }
    try {
        socket->send(msg);
    } catch (const zappy::exception::SocketError& /*err */) {
        return false;
    }
    return true;
}

}  // namespace zappy::server
