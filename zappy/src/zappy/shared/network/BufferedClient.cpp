/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** BufferedClient
*/

#include "BufferedClient.hpp"

#include <cstddef>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::network {

void BufferedClient::connect(const Address& address) const { _client.connect(address); }

int BufferedClient::fd() const { return _client.fd(); }

bool BufferedClient::hasMessages() const { return !_messages.empty(); }

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
std::string BufferedClient::popMessage() {
    if (_messages.empty()) {
        throw exception::SocketError{"No buffered message available"};
    }
    std::string message = std::move(_messages.front());
    _messages.pop();
    return message;
}

void BufferedClient::poll() {
    auto bytes = _client.read(4096);

    if (bytes.empty() && !_client.isOpen()) {
        throw exception::SocketError{"Connection to server lost"};
    }

    _buffer.reserve(_buffer.size() + bytes.size());
    for (std::byte b : bytes) _buffer += static_cast<char>(std::to_underlying(b));

    std::size_t pos = std::string::npos;  // NOLINT(*-init-variables)
    while ((pos = _buffer.find('\n')) != std::string::npos) {
        _messages.push(_buffer.substr(0, pos));
        _buffer.erase(0, pos + 1);
    }
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void BufferedClient::send(std::string_view line) {
    auto bytes = std::as_bytes(std::span{line});
    std::size_t sent = 0;
    while (sent < bytes.size()) {
        const std::size_t chunk = _client.send(bytes.subspan(sent));  // NOLINT(*-init-variables)
        if (chunk == 0) {
            throw exception::SocketError{"Connection to server lost"};
        }
        sent += chunk;
    }
}

}  // namespace zappy::network
