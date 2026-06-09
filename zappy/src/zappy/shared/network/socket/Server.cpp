/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ServerSocket
*/

#include "zappy/shared/network/socket/Server.hpp"

#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <cstdint>
#include <memory>
#include <system_error>

#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/socket/Client.hpp"
#include "zappy/shared/network/socket/IClient.hpp"

namespace zappy::network::socket {
void Server::listen(const std::uint16_t maxConnections) {
    _maxConnections = maxConnections;
    if (::listen(fd(), _maxConnections) == -1) {
        const std::error_code error{errno, std::generic_category()};

        throw exception::SocketError{"Failed to listen on socket: " + error.message()};
    }
}

std::unique_ptr<IClient> Server::accept() const {
    sockaddr_in address{};
    socklen_t len = sizeof(address);
    const int client =
        // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
        ::accept(fd(), reinterpret_cast<sockaddr*>(&address), &len);

    if (client == -1 && errno != EINTR) {
        const std::error_code error{errno, std::generic_category()};

        throw exception::SocketError{"Failed to accept connection: " + error.message()};
    }
    return std::make_unique<Client>(client, address);
}
}  // namespace zappy::network::socket
