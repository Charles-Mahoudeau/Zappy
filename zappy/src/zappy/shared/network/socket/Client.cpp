/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ClientSocket
*/

#include "zappy/shared/network/socket/Client.hpp"

#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <system_error>

#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::network::socket {
void Client::connect(const Address& address) const {
    sockaddr_in targetAddress = address.sockaddr();

    // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
    if (::connect(fd(), reinterpret_cast<sockaddr*>(&targetAddress), sizeof(targetAddress)) == -1) {
        const std::error_code error{errno, std::generic_category()};

        throw exception::SocketError{"Failed to connect to server: " + error.message()};
    }
}
}  // namespace zappy::network::socket
