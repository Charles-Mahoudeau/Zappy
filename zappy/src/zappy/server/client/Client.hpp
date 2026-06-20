/*
** EPITECH PROJECT, 2026
** IClient
** File description:
** IClient header
*/

#pragma once

#include <cstdint>
#include <optional>
#include <queue>
#include <string>
#include <string_view>

#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server {

class Client {
  public:
    enum class Type : std::uint8_t {
        kPlayer,
        kGui,
        kUnknown,
    };

    Client(net::SocketRegistry& socketRegister, network::Address address);
    ~Client() = default;

    Client(const Client&) = delete;
    Client(Client&&) = delete;
    Client& operator=(const Client&) = delete;
    Client& operator=(Client&&) = delete;

    bool update();

    [[nodiscard]] const network::Address& address() const;
    [[nodiscard]] Type type() const;
    void changeType(Type type);

    void addRequest(std::string msg);
    std::optional<std::string> nextRequest();
    void setTimeout(int timeout);

    bool sendMessage(std::string_view msg);

  private:
    int _timeout = 0;
    std::queue<std::string> _requests;
    Type _type = Client::Type::kUnknown;
    network::Address _addr;
    net::SocketRegistry& _socketsRegistery;

    static constexpr int kMAX_REQUEST = 10;
};

}  // namespace zappy::server
