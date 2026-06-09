/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Address
*/

#pragma once

#include <array>
#include <cstdint>
#include <ostream>
#include <string>
#include <string_view>

namespace zappy::network {
class Address {
  public:
    Address() = default;
    Address(const std::string& ip, std::uint16_t port);
    Address(const std::array<std::uint8_t, 4>& ip, std::uint16_t port);
    Address(const Address& other) = default;
    Address(Address&& other) = default;
    ~Address() = default;

    Address& operator=(Address&& other) = default;
    Address& operator=(const Address& other) = default;

    explicit operator std::string() const;

    bool operator==(const Address&) const = default;

    [[nodiscard]] std::string string() const;

    [[nodiscard]] std::array<std::uint8_t, 4> ip() const;
    [[nodiscard]] std::string ipString() const;
    [[nodiscard]] std::uint16_t port() const;

  private:
    std::array<std::uint8_t, 4> _ip{0, 0, 0, 0};
    std::uint16_t _port{0};
};

std::ostream& operator<<(std::ostream& os, const Address& address);
}  // namespace zappy::network
