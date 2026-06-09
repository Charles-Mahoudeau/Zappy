/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Address
*/

#include "Address.hpp"

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
Address::Address(const std::array<std::uint8_t, 4>& ip, const std::uint16_t port) : _ip{ip}, _port{port} {}

Address::Address(const std::string& ip, const std::uint16_t port) : _ip{}, _port{port} {
    std::vector<std::uint8_t> ipParts;
    std::string part;
    std::stringstream ipStream{ip};

    while (std::getline(ipStream, part, '.')) {
        const int val = std::stoi(part);

        if (val < 0 || val > 255) {
            throw exception::InvalidAddress{"Invalid ip component: " + ip};
        }
        ipParts.push_back(static_cast<std::uint8_t>(val));
    }
    if (ipParts.size() != 4) {
        throw exception::InvalidAddress{"Invalid ip: " + ip};
    }
    _ip = {ipParts.at(0), ipParts.at(1), ipParts.at(2), ipParts.at(3)};
}

Address::operator std::string() const { return string(); }

std::string Address::tuple() const {
    const std::uint8_t p1 = _port / 256;
    const std::uint8_t p2 = _port % 256;

    return std::format("{},{},{},{},{},{}", _ip.at(0), _ip.at(1), _ip.at(2), _ip.at(3), p1, p2);
}

void Address::fromTuple(const std::string_view tuple) {
    std::stringstream stream{std::string{tuple}};
    std::vector<std::uint8_t> parts;
    std::string part;

    while (std::getline(stream, part, ',')) {
        parts.push_back(std::stoi(part));
    }
    if (parts.size() != 6) {
        throw exception::InvalidAddress{tuple};
    }
    _ip = {parts.at(0), parts.at(1), parts.at(2), parts.at(3)};
    _port = static_cast<std::uint16_t>((parts.at(4) * 256) + parts.at(5));
}

std::string Address::string() const {
    return std::format("{}.{}.{}.{}:{}", _ip.at(0), _ip.at(1), _ip.at(2), _ip.at(3), _port);
}

std::array<std::uint8_t, 4> Address::ip() const { return _ip; }

std::string Address::ipString() const {
    return std::format("{}.{}.{}.{}", _ip.at(0), _ip.at(1), _ip.at(2), _ip.at(3));
}

std::uint16_t Address::port() const { return _port; }

std::ostream& operator<<(std::ostream& os, const Address& address) {
    os << address.string();
    return os;
}
}  // namespace zappy::network
