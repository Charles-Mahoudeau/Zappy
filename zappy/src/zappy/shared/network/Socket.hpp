/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Socket
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "Address.hpp"
#include "ISocket.hpp"

namespace zappy::network {
class Socket : public ISocket {
  public:
    Socket();
    Socket(int socket, const Address& address);
    ~Socket() override;

    Socket(const Socket& other) noexcept = delete;
    Socket& operator=(const Socket& other) noexcept = delete;

    Socket(Socket&& other) noexcept;
    Socket& operator=(Socket&& other) noexcept;

    [[nodiscard]] int fd() const override;

    [[nodiscard]] Address address() const override;
    [[nodiscard]] bool isOpen() const override;

    void bind(std::uint16_t port) override;

    void close() override;

    [[nodiscard]] std::vector<std::byte> read(std::size_t count) override;

    [[nodiscard]] std::size_t send(std::span<const std::byte> data) override;

  private:
    int _socket{-1};
    Address _address;
};
}  // namespace zappy::network
