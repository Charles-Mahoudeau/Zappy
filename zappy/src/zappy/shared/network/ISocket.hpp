/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ISocket
*/

#pragma once

#include <cstddef>
#include <cstdint>
#include <ostream>
#include <span>
#include <vector>

#include "Address.hpp"
#include "zappy/shared/io/IFileDescriptor.hpp"

namespace zappy::network {
class ISocket : public io::IFileDescriptor {
  public:
    ISocket() = default;
    ISocket(const ISocket& other) noexcept = default;
    ISocket(ISocket&& other) noexcept = default;
    ~ISocket() override = default;

    ISocket& operator=(const ISocket& other) noexcept = default;
    ISocket& operator=(ISocket&& other) noexcept = default;

    [[nodiscard]] int fd() const override = 0;

    [[nodiscard]] virtual Address address() const = 0;
    [[nodiscard]] virtual bool isOpen() const = 0;

    virtual void bind(std::uint16_t port) = 0;

    virtual void close() = 0;

    [[nodiscard]] virtual std::vector<std::byte> read(std::size_t count) = 0;

    [[nodiscard]] virtual std::size_t send(std::span<const std::byte> data) = 0;
};

inline std::ostream& operator<<(std::ostream& os, const ISocket& socket) {
    os << socket.address();
    return os;
}
}  // namespace zappy::network
