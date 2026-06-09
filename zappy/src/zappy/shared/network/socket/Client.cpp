/*
** EPITECH PROJECT, 2026
** myteams
** File description:
** ClientSocket
*/

#include "Client.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <cerrno>
#include <system_error>

#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::network::socket {
void Client::connect(const Address& address) {
    sockaddr_in targetAddress = {
        .sin_family = AF_INET,
        .sin_port = htons(address.port()),
        .sin_addr =
            {
                .s_addr = inet_addr(address.ipString().c_str()),
            },
    };
    // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
    if (::connect(fd(), reinterpret_cast<sockaddr*>(&targetAddress), sizeof(targetAddress)) == -1) {
        const std::error_code error{errno, std::generic_category()};

        throw exception::SocketError{"Failed to connect to server: " + error.message()};
    }
}
}  // namespace zappy::network::socket
