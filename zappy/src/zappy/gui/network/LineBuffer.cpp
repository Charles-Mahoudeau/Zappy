/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** LineBuffer
*/

#include "LineBuffer.hpp"

#include <cstddef>
#include <span>
#include <string>
#include <string_view>

#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::gui::network {

void LineBuffer::connect(const zappy::network::Address& address) { _client.connect(address); }

int LineBuffer::fd() const { return _client.fd(); }

bool LineBuffer::hasMessages() const { return !_messages.empty(); }

std::string LineBuffer::popMessage() {
    std::string message = std::move(_messages.front());
    _messages.pop();
    return message;
}

void LineBuffer::poll() {
    auto bytes = _client.read(4096);

    if (bytes.empty() && !_client.isOpen()) {
        throw exception::SocketError{"Connection to server lost"};
    }

    // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
    _buffer.append(reinterpret_cast<const char*>(bytes.data()), bytes.size());

    std::size_t pos = 0;
    while ((pos = _buffer.find('\n')) != std::string::npos) {
        _messages.push(_buffer.substr(0, pos));
        _buffer.erase(0, pos + 1);
    }
}

void LineBuffer::send(std::string_view line) {
    // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
    const auto* ptr = reinterpret_cast<const std::byte*>(line.data());
    std::size_t sent = 0;
    while (sent < line.size()) {
        sent += _client.send(std::span<const std::byte>{ptr + sent, line.size() - sent});
    }
}

}  // namespace zappy::gui::network
