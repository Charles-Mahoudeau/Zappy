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

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server::client {

class ClientRegistry {
  public:
    ClientRegistry() = default;
    ~ClientRegistry() = default;

    ClientRegistry(const ClientRegistry&) = delete;
    ClientRegistry(ClientRegistry&&) = delete;
    ClientRegistry& operator=(const ClientRegistry&) = delete;
    ClientRegistry& operator=(ClientRegistry&&) = delete;

    void makeNewClient(net::SocketRegistry& socketRegistery, network::Address addr, Timer& timer);

    /**
     * @brief Update all managed clients and remove any that are no longer alive.
     *
     * @details This method performs a full tick:
     *   1. Re-evaluates which type group each client belongs to (e.g. a client
     *      may have been promoted from kUnknown to kPlayer).
     *   2. Calls Client::update() on every managed client. Any client that
     *      returns false (indicating disconnection or fatal error) is marked
     *      for removal immediately.
     *   3. Removes all marked clients from the registry in a second pass,
     *      leaving the container in a valid state.
     *
     * Call this once per server tick.
     */
    void update();

    /**
     * @brief Mark a specific client for removal.
     *
     * @param clientPtr Pointer to the Client to remove. no-op if nullptr.
     *
     * @details The removal is stored until the next call to update() so that
     * iterators over the client container remain valid during a tick.
     */
    void markForRemoval(const Client* clientPtr);

    /**
     * @brief Obtain a range view over all managed clients.
     *
     * @return A range (std::ranges::range) yielding raw pointers to every
     *         Client owned by the registry.
     */
    auto viewAll();

    /**
     * @brief Obtain a range view over all managed clients of a given type.
     *
     * @param type The Client::Type to filter on (e.g. Client::Type::kPlayer).
     *
     * @return A range yielding raw pointers to every Client of
     *         the specified type.
     */
    auto viewAll(Client::Type type);

    Client* findByAddress(const network::Address& addr);

  private:
    std::vector<const Client*> _toRemove;
    std::vector<std::unique_ptr<Client>> _clients;
    std::unordered_map<Client::Type, std::vector<const Client*>> _clientsPerType = {
        {Client::Type::kUnknown, {}},
        {Client::Type::kGui, {}},
        {Client::Type::kPlayer, {}},
    };

    void updateTypeGroup();
};

inline auto ClientRegistry::viewAll() {
    return this->_clients | std::views::transform([](const std::unique_ptr<Client>& client) { return client.get(); });
}

inline auto ClientRegistry::viewAll(Client::Type type) { return this->_clientsPerType.at(type) | std::views::all; }

}  // namespace zappy::server::client
