/*
** EPITECH PROJECT, 2026
** ClientsRegistry
** File description:
** ClientsRegistry code
*/

#include "zappy/server/client/ClientsRegistry.hpp"

#include <memory>
#include <utility>
#include <vector>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"
namespace zappy::server::client {

void ClientsRegistry::makeNewClient(net::SocketRegistry& socketRegistery, network::Address addr) {
    std::unique_ptr<Client> newClient = std::make_unique<Client>(socketRegistery, addr);

    this->_clientsPerType.at(newClient->type()).emplace_back(newClient.get());
    this->_clients.emplace_back(std::move(newClient));
}

void ClientsRegistry::remove(Client* clientPtr) {
    if (clientPtr == nullptr) {
        return;
    }

    std::vector<const Client*>& typedList = this->_clientsPerType.at(clientPtr->type());
    std::erase_if(typedList, [&clientPtr](const Client* client) { return client == clientPtr; });
    std::erase_if(this->_clients,
                  [&clientPtr](const std::unique_ptr<Client>& client) { return client.get() == clientPtr; });
}

void ClientsRegistry::update() {
    std::vector<Client*> toRemove;

    for (const auto& client : this->_clients) {
        if (!client->update()) {
            toRemove.push_back(client.get());
        }
    }

    for (Client* client : toRemove) {
        this->remove(client);
    }
}

}  // namespace zappy::server::client
