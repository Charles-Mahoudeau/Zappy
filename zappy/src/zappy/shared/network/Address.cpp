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
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "zappy/shared/exception/InvalidAddress.hpp"

namespace zappy::network {
Address::Address(const std::array<std::uint8_t, 4>& ip, const std::uint16_t port) : _ip{ip}, _port{port} {}

Address::Address(const std::string& ip, std::uint16_t port) : _ip{}, _port{port} {
    std::vector<std::uint8_t> ip_parts;
    std::string part;
    std::stringstream ip_stream(ip);

    while (std::getline(ip_stream, part, '.')) {
        ip_parts.push_back(static_cast<std::uint8_t>(std::strtol(part.c_str(), nullptr, 10)));
    }
    if (ip_parts.size() != 4) {
        throw exception::InvalidAddress("Invalid IP : " + ip);
    }
    _ip = {ip_parts.at(0), ip_parts.at(1), ip_parts.at(2), ip_parts.at(3)};
}

Address::operator std::string() const { return string(); }

std::string Address::tuple() const {
    const std::uint8_t p1 = _port / 256;
    const std::uint8_t p2 = _port % 256;
    return std::to_string(_ip.at(0)) + "," + std::to_string(_ip.at(1)) + "," + std::to_string(_ip.at(2)) + "," +
           std::to_string(_ip.at(3)) + "," + std::to_string(p1) + "," + std::to_string(p2);
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
    return std::to_string(_ip.at(0)) + "." + std::to_string(_ip.at(1)) + "." + std::to_string(_ip.at(2)) + "." +
           std::to_string(_ip.at(3)) + ":" + std::to_string(_port);
}

std::array<std::uint8_t, 4> Address::ip() const { return _ip; }

std::string Address::ipString() const {
    return std::to_string(_ip.at(0)) + "." + std::to_string(_ip.at(1)) + "." + std::to_string(_ip.at(2)) + "." +
           std::to_string(_ip.at(3));
}

std::uint16_t Address::port() const { return _port; }

std::ostream& operator<<(std::ostream& os, const Address& address) {
    os << address.string();
    return os;
}
}  // namespace zappy::network
