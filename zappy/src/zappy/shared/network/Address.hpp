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

    // ReSharper disable once CppNonExplicitConversionOperator
    operator std::string() const;

    bool operator==(const Address&) const = default;

    [[nodiscard]] std::string tuple() const;
    void fromTuple(std::string_view tuple);

    [[nodiscard]] std::string string() const;

    [[nodiscard]] std::array<std::uint8_t, 4> ip() const;
    [[nodiscard]] std::string ipString() const;
    [[nodiscard]] std::uint16_t port() const;

  private:
    std::array<std::uint8_t, 4> _ip;
    std::uint16_t _port;
};

std::ostream& operator<<(std::ostream& os, const Address& address);
}  // namespace zappy::network
