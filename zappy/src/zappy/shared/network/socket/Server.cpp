/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ServerSocket
*/

#include "zappy/shared/network/socket/Server.hpp"

#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstdint>
#include <system_error>

#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace zappy::network::socket {
void Server::listen(const std::uint16_t maxConnections) {
    _maxConnections = maxConnections;
    if (::listen(fd(), _maxConnections) == -1) {
        const std::error_code error{errno, std::generic_category()};

        throw exception::SocketError{"Failed to listen on socket: " + error.message()};
    }
}

Client Server::accept() const {
    sockaddr_in address{};
    socklen_t len = sizeof(address);

    for (std::uint8_t attemptsRemaining{kAcceptMaximumAttempts}; attemptsRemaining > 0; --attemptsRemaining) {
        // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
        if (const int client = ::accept(fd(), reinterpret_cast<sockaddr*>(&address), &len); client != -1) {
            return {client, Address{address}};
        }
        if (errno == EAGAIN) {
            continue;
        }

        const std::error_code error{errno, std::generic_category()};

        throw exception::SocketError{"Failed to accept connection: " + error.message()};
    }
    throw exception::SocketError{"Failed to accept connection: maximum attempts reached."};
}
}  // namespace zappy::network::socket
