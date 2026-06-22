
/*
** EPITECH PROJECT, 2026
** ClientRegistry tests
*/

#include "zappy/server/client/ClientRegistry.hpp"

#include <gtest/gtest.h>

#include <iterator>
#include <ranges>

#include "SocketPair.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/socket/Client.hpp"

using namespace zappy::server;
using namespace zappy::server::client;

namespace {

class ClientRegistryTest : public ::testing::Test {
  public:
    SocketPair pair;
    net::SocketRegistry socketRegistry;
    zappy::network::Address addr{"127.0.0.1", 4242};
    ClientRegistry registry;

  protected:
    void SetUp() override {
        zappy::network::socket::Client sock{pair.local, addr};
        socketRegistry.insert(sock);
    }
};

}  // namespace

// ── makeNewClient ────────────────────────────────────────────────────────────

TEST_F(ClientRegistryTest, MakeNewClientIncreasesViewAllSize) {
    registry.makeNewClient(socketRegistry, addr);

    const auto all = registry.viewAll();
    EXPECT_EQ(std::ranges::distance(all), 1);
}

TEST_F(ClientRegistryTest, MakeNewClientAppearsInType) {
    registry.makeNewClient(socketRegistry, addr);

    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kUnknown)), 1);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kGui)), 0);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kPlayer)), 0);
}

TEST_F(ClientRegistryTest, MakeMultipleClientsAllVisible) {
    registry.makeNewClient(socketRegistry, addr);
    registry.makeNewClient(socketRegistry, addr);
    registry.makeNewClient(socketRegistry, addr);

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 3);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kUnknown)), 3);
}

// ── remove ───────────────────────────────────────────────────────────────────

TEST_F(ClientRegistryTest, RemoveClientDecreasesSize) {
    registry.makeNewClient(socketRegistry, addr);

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 1);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kUnknown)), 1);
    const Client* ptr = *registry.viewAll().begin();
    registry.markForRemoval(ptr);

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 1);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kUnknown)), 1);

    registry.update();

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 0);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(Client::Type::kUnknown)), 0);
}

TEST_F(ClientRegistryTest, RemoveOneOfManyLeavesOthers) {
    registry.makeNewClient(socketRegistry, addr);
    registry.makeNewClient(socketRegistry, addr);

    const Client* ptr = *registry.viewAll().begin();
    registry.markForRemoval(ptr);
    registry.update();

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 1);
}

TEST_F(ClientRegistryTest, RemoveUnknownPtrDoesNothing) {
    registry.makeNewClient(socketRegistry, addr);

    const Client fake{socketRegistry, addr};
    registry.markForRemoval(&fake);  // not owned by registry
    registry.update();

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 1);
}

// ── update ───────────────────────────────────────────────────────────────────

TEST_F(ClientRegistryTest, UpdateKeepsHealthyClients) {
    registry.makeNewClient(socketRegistry, addr);

    EXPECT_NO_THROW(registry.update());
    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 1);
}

TEST_F(ClientRegistryTest, UpdateRemovesClientWhenPeerCloses) {
    registry.makeNewClient(socketRegistry, addr);

    socketRegistry.clear();

    registry.update();

    EXPECT_EQ(std::ranges::distance(registry.viewAll()), 0);
}

TEST_F(ClientRegistryTest, UpdateEmptyRegistryDoesNotThrow) { EXPECT_NO_THROW(registry.update()); }

TEST_F(ClientRegistryTest, UpdateClientType) {
    registry.makeNewClient(socketRegistry, addr);

    EXPECT_EQ(std::ranges::distance(registry.viewAll(zappy::server::Client::Type::kUnknown)), 1);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(zappy::server::Client::Type::kGui)), 0);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(zappy::server::Client::Type::kPlayer)), 0);

    registry.viewAll().front()->changeType(zappy::server::Client::Type::kGui);

    registry.update();

    EXPECT_EQ(std::ranges::distance(registry.viewAll(zappy::server::Client::Type::kUnknown)), 0);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(zappy::server::Client::Type::kGui)), 1);
    EXPECT_EQ(std::ranges::distance(registry.viewAll(zappy::server::Client::Type::kPlayer)), 0);
}

// ── viewAll(
