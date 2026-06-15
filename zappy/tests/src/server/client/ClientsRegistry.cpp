
/*
** EPITECH PROJECT, 2026
** ClientsRegistry tests
*/

#include "zappy/server/client/ClientsRegistry.hpp"

#include <gtest/gtest.h>
#include <unistd.h>

#include <iterator>
#include <ranges>

#include "SocketPair.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/socket/Client.hpp"

using namespace zappy::server;
using namespace zappy::server::client;

class ClientsRegistryTest : public ::testing::Test {
  public:
    SocketPair pair;
    net::SocketRegistry socketRegistry;
    zappy::network::Address addr{"127.0.0.1", 4242};
    ClientsRegistry registry;

  protected:
    void SetUp() override {
        zappy::network::socket::Client sock{pair.local, addr};
        socketRegistry.insert(sock);
    }
};

// ── makeNewClient ────────────────────────────────────────────────────────────

TEST_F(ClientsRegistryTest, MakeNewClientIncreasesViewAllSize) {
    registry.makeNewClient(socketRegistry, addr);

    const auto all = registry.viewAll();
    EXPECT_EQ(std::ranges::distance(all), 1);
}

TEST_F(ClientsRegistryTest, MakeNewClientAppearsInType) {
    registry.makeNewClient(socketRegistry, addr);

    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kUNKNOWN)), 1);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kGUI)), 0);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kPLAYER)), 0);
}

TEST_F(ClientsRegistryTest, MakeMultipleClientsAllVisible) {
    registry.makeNewClient(socketRegistry, addr);
    registry.makeNewClient(socketRegistry, addr);
    registry.makeNewClient(socketRegistry, addr);

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 3);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kUNKNOWN)), 3);
}

// ── remove ───────────────────────────────────────────────────────────────────

TEST_F(ClientsRegistryTest, RemoveNullptrDoesNotThrow) { EXPECT_NO_THROW(registry.remove(nullptr)); }

TEST_F(ClientsRegistryTest, RemoveClientDecreasesSize) {
    registry.makeNewClient(socketRegistry, addr);

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 1);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kUNKNOWN)), 1);
    Client* ptr = *registry.viewAll().begin();
    registry.remove(ptr);

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 0);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kUNKNOWN)), 0);
}

TEST_F(ClientsRegistryTest, RemoveOneOfManyLeavesOthers) {
    registry.makeNewClient(socketRegistry, addr);
    registry.makeNewClient(socketRegistry, addr);

    Client* ptr = *registry.viewAll().begin();
    registry.remove(ptr);

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 1);
}

TEST_F(ClientsRegistryTest, RemoveUnknownPtrDoesNothing) {
    registry.makeNewClient(socketRegistry, addr);

    Client fake{socketRegistry, addr};
    registry.remove(&fake);  // not owned by registry

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 1);
}

// ── update ───────────────────────────────────────────────────────────────────

TEST_F(ClientsRegistryTest, UpdateKeepsHealthyClients) {
    registry.makeNewClient(socketRegistry, addr);

    EXPECT_NO_THROW(registry.update());
    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 1);
}

// TODO
TEST_F(ClientsRegistryTest, UpdateRemovesClientWhenPeerCloses) {
    registry.makeNewClient(socketRegistry, addr);

    socketRegistry.clear();

    registry.update();

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 0);
}

TEST_F(ClientsRegistryTest, UpdateEmptyRegistryDoesNotThrow) { EXPECT_NO_THROW(registry.update()); }

// ── viewAll(
