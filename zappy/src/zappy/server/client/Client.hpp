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

#include "zappy/server/network/SocketRegistery.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server {

class Client {
  public:
    enum class Type : std::uint8_t {
        kPLAYER,
        kGUI,
        kUNKNOWN,
    };

    Client(net::SocketRegistery& socketRegister, network::Address address);
    ~Client() = default;

    Client(const Client&) = delete;
    Client(Client&&) = delete;
    Client& operator=(const Client&) = delete;
    Client& operator=(Client&&) = delete;

    bool update();

    network::Address& address();
    Type type();
    void changeType(Type type);

    void addRequest(std::string msg);
    std::optional<std::string> getNextRequest();
    void setTimeout(int timeout);

    void sendMessage(std::string_view msg);

  private:
    int _timeout = 0;
    std::queue<std::string> _requests;
    Type _type = Client::Type::kUNKNOWN;
    network::Address _addr;
    net::SocketRegistery& _socketsRegistery;

    static constexpr int kMAX_REQUEST = 10;
};

}  // namespace zappy::server
