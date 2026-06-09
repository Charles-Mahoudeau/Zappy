/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Socket
*/

#include "Socket.hpp"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <span>
#include <system_error>
#include <type_traits>
#include <vector>

#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::network {
Socket::Socket() : _socket{socket(AF_INET, SOCK_STREAM, 0)} {
    if (_socket == -1) {
        const std::error_code error{errno, std::generic_category()};

        throw exception::SocketError{"Failed to create socket: " + error.message()};
    }

    constexpr int opt = 1;

    // NOLINTNEXTLINE(misc-include-cleaner)
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        const std::error_code error{errno, std::generic_category()};
        throw exception::SocketError{"Failed to set socket options: " + error.message()};
    }
}

Socket::Socket(const int socket, Address address) : _socket{socket}, _address{std::move(address)} {}

Socket::Socket(Socket&& other) noexcept : _socket{other._socket}, _address{other._address} {
    other._socket = -1;
    other._address = {};
}

Socket& Socket::operator=(Socket&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    close();
    _socket = other._socket;
    _address = other._address;
    other._socket = -1;
    other._address = {};
    return *this;
}

Socket::~Socket() { Socket::close(); }

int Socket::fd() const { return _socket; }

Address Socket::address() const {
    return _address;
}

bool Socket::isOpen() const { return _socket != -1; }

void Socket::bind(const std::uint16_t port) {
    _address = Address{{
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr =
            {
                .s_addr = htonl(INADDR_ANY),
            },
    }};
    // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
    if (::bind(_socket, reinterpret_cast<sockaddr*>(&_address), sizeof(_address)) == -1) {
        const std::error_code error{errno, std::generic_category()};

        throw exception::SocketError{"Failed to bind socket: " + error.message()};
    }
}

void Socket::close() {
    if (_socket == -1) {
        return;
    }
    ::close(_socket);
    _socket = -1;
}

std::vector<std::byte> Socket::read(const std::size_t count) {
    std::vector<std::byte> buffer;

    buffer.resize(count);

    const std::make_signed_t<std::size_t> bytesRead = ::read(_socket, buffer.data(), count);

    if (bytesRead == -1) {
        const std::error_code error{errno, std::generic_category()};

        close();
        throw exception::SocketError{"Failed to read from socket: " + error.message()};
    }
    if (bytesRead == 0) {
        close();
    }
    buffer.resize(bytesRead);
    return buffer;
}

std::size_t Socket::send(const std::span<const std::byte> data) {
    const std::make_signed_t<std::size_t> res = write(_socket, data.data(), data.size_bytes());

    if (res == -1) {
        const std::error_code error{errno, std::generic_category()};

        close();
        throw exception::SocketError{"Failed to send data: " + error.message()};
    }
    return res;
}

void Socket::fetch() {
    socklen_t addrlen = sizeof(_address);

    // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
    if (getsockname(_socket, reinterpret_cast<sockaddr*>(&_address), &addrlen) == -1) {
        const std::error_code error{errno, std::generic_category()};

        throw exception::SocketError{"Failed to fetch socket address: " + error.message()};
    }
}
}  // namespace zappy::network
