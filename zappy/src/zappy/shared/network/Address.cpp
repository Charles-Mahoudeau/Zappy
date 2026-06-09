/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Address
*/

#include "Address.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <array>
#include <cstdint>
#include <format>
#include <ostream>
#include <string>

#include "zappy/shared/exception/InvalidAddress.hpp"

namespace zappy::network {
Address::Address(const sockaddr_in sockaddr) : _sockaddr{sockaddr}, _port{ntohs(_sockaddr.sin_port)} {
    const uint32_t host_addr = ntohl(_sockaddr.sin_addr.s_addr);

    _ip = {
        static_cast<uint8_t>((host_addr >> 24) & 0xFF),
        static_cast<uint8_t>((host_addr >> 16) & 0xFF),
        static_cast<uint8_t>((host_addr >> 8) & 0xFF),
        static_cast<uint8_t>(host_addr & 0xFF),
    };
}

Address::Address(const std::string& ip, const std::uint16_t port) : _port{port} {
    _sockaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &_sockaddr.sin_addr) != 1) {
        throw exception::InvalidAddress{"Invalid IP address: " + ip};
    }

    const uint32_t host_addr = ntohl(_sockaddr.sin_addr.s_addr);

    _ip = {
        static_cast<uint8_t>((host_addr >> 24) & 0xFF),
        static_cast<uint8_t>((host_addr >> 16) & 0xFF),
        static_cast<uint8_t>((host_addr >> 8) & 0xFF),
        static_cast<uint8_t>(host_addr & 0xFF),
    };
}

Address::operator std::string() const { return string(); }

bool Address::operator==(const Address& other) const { return _ip == other._ip && _port == other._port; }

std::string Address::string() const {
    return std::format("{}.{}.{}.{}:{}", _ip.at(0), _ip.at(1), _ip.at(2), _ip.at(3), _port);
}

sockaddr_in Address::sockaddr() const { return _sockaddr; }

std::array<std::uint8_t, 4> Address::ip() const { return _ip; }

std::string Address::ipString() const { return std::format("{}.{}.{}.{}", _ip.at(0), _ip.at(1), _ip.at(2), _ip.at(3)); }

std::uint16_t Address::port() const { return _port; }

std::ostream& operator<<(std::ostream& os, const Address& address) {
    os << address.string();
    return os;
}
}  // namespace zappy::network
