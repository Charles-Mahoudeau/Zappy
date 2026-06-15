/*
** EPITECH PROJECT, 2026
** ClientRegister
** File description:
** ClientRegister header
*/

#pragma once

#include <memory>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server::client {

class ClientsRegistry {
  public:
    ClientsRegistry() = default;
    ~ClientsRegistry() = default;

    ClientsRegistry(const ClientsRegistry&) = delete;
    ClientsRegistry(ClientsRegistry&&) = delete;
    ClientsRegistry& operator=(const ClientsRegistry&) = delete;
    ClientsRegistry& operator=(ClientsRegistry&&) = delete;

    void makeNewClient(net::SocketRegistry& socketRegistery, network::Address addr);

    void update();

    void remove(Client* clientPtr);

    auto viewAll();

    auto viewAll(Client::Type type);

  private:
    std::vector<std::unique_ptr<Client>> _clients;
    std::unordered_map<Client::Type, std::vector<const Client*>> _clientsPerType = {
        {Client::Type::kUNKNOWN, {}},
        {Client::Type::kGUI, {}},
        {Client::Type::kPLAYER, {}},
    };
};

inline auto ClientsRegistry::viewAll() {
    return this->_clients | std::views::transform([](const std::unique_ptr<Client>& client) { return client.get(); });
}

inline auto ClientsRegistry::viewAll(Client::Type type) { return this->_clientsPerType.at(type) | std::views::all; }

}  // namespace zappy::server::client
