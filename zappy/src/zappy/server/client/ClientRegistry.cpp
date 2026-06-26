/*
** EPITECH PROJECT, 2026
** ClientRegistry
** File description:
** ClientRegistry code
*/

#include "zappy/server/client/ClientRegistry.hpp"

#include <algorithm>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server::client {

void ClientRegistry::makeNewClient(net::SocketRegistry& socketRegistery, network::Address addr, Timer& timer) {
    auto newClient = std::make_unique<Client>(socketRegistery, addr, timer);

    this->_clientsPerType.at(newClient->type()).emplace_back(newClient.get());
    this->_clients.emplace_back(std::move(newClient));
}

void ClientRegistry::markForRemoval(const Client* clientPtr) {
    if (clientPtr == nullptr) {
        return;
    }
    this->_toRemove.emplace_back(clientPtr);
}

void ClientRegistry::update() {
    this->updateTypeGroup();

    for (const auto& client : this->_clients) {
        if (!client->update()) {
            this->_toRemove.emplace_back(client.get());
            continue;
        }
    }

    for (const Client* clientPtr : this->_toRemove) {
        std::vector<Client*>& typedList = this->_clientsPerType.at(clientPtr->type());
        std::erase_if(typedList, [&clientPtr](const Client* client) { return client == clientPtr; });
        std::erase_if(this->_clients,
                      [&clientPtr](const std::unique_ptr<Client>& client) { return client.get() == clientPtr; });
    }
    this->_toRemove.clear();
}

void ClientRegistry::updateTypeGroup() {
    std::vector<std::pair<Client::Type, Client*>> toMove;

    for (auto& [groupType, groupList] : this->_clientsPerType) {
        for (Client* client : groupList) {
            if (groupType != client->type()) {
                toMove.emplace_back(groupType, client);
            }
        }
    }

    for (auto& [fromType, client] : toMove) {
        this->_clientsPerType.at(client->type()).emplace_back(client);
        auto& src = this->_clientsPerType.at(fromType);
        std::erase_if(src, [client](const Client* InnerClient) { return client == InnerClient; });
    }
}

Client* ClientRegistry::findByAddress(const network::Address& addr) {
    if (auto iter = std::ranges::find_if(
            this->_clients, [&addr](const std::unique_ptr<Client>& client) { return client->address() == addr; });
        iter != this->_clients.end()) {
        return iter.base()->get();
    }
    return nullptr;
}

Client* ClientRegistry::findByID(std::uint64_t id) {
    auto& clients = this->_clientsPerType.at(Client::Type::kPlayer);

    for (auto& client : clients) {
        if (client->playerID() == id) {
            return client;
        }
    }
    return nullptr;
}

}  // namespace zappy::server::client
