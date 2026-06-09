/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Address
*/

#pragma once

#include <netinet/in.h>

#include <array>
#include <cstdint>
#include <ostream>
#include <string>

namespace zappy::network {
class Address {
  public:
    Address() = default;
    explicit Address(sockaddr_in sockaddr);
    Address(const std::string& ip, std::uint16_t port);
    ~Address() = default;

    Address(const Address& other) = default;
    Address& operator=(Address&& other) = default;

    Address(Address&& other) = default;
    Address& operator=(const Address& other) = default;

    explicit operator std::string() const;

    bool operator==(const Address& other) const;

    [[nodiscard]] std::string string() const;

    [[nodiscard]] sockaddr_in sockaddr() const;
    [[nodiscard]] std::array<std::uint8_t, 4> ip() const;
    [[nodiscard]] std::string ipString() const;
    [[nodiscard]] std::uint16_t port() const;

  private:
    sockaddr_in _sockaddr{
        .sin_family = AF_INET,
        .sin_port = 0,
        .sin_addr = {},
    };
    std::array<std::uint8_t, 4> _ip{0, 0, 0, 0};
    std::uint16_t _port{0};
};

std::ostream& operator<<(std::ostream& os, const Address& address);
}  // namespace zappy::network
