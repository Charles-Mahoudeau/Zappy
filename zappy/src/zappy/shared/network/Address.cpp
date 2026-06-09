/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Address
*/

#include "Address.hpp"

#include <arpa/inet.h>

#include <array>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "zappy/shared/exception/InvalidAddress.hpp"

namespace zappy::network {
Address::Address(sockaddr_in sockaddr) : _sockaddr{std::move(sockaddr)}, _port{_sockaddr.sin_port} {
    const auto bytes = std::bit_cast<std::array<uint8_t, 4>>(_sockaddr.sin_addr.s_addr);

    _ip = {bytes.at(0), bytes.at(1), bytes.at(2), bytes.at(3)};
}

Address::Address(const std::string& ip, const std::uint16_t port) : _port{port} {
    _sockaddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &_sockaddr.sin_addr) != 1) {
        throw exception::InvalidAddress{"Invalid IP address: " + ip};
    }

    const auto bytes = std::bit_cast<std::array<uint8_t, 4>>(_sockaddr.sin_addr.s_addr);

    _ip = {bytes.at(0), bytes.at(1), bytes.at(2), bytes.at(3)};
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
