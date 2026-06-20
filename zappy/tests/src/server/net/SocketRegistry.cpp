/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** SocketRegistry tests
*/

#include "zappy/server/net/SocketRegistry.hpp"

#include <gtest/gtest.h>

#include <cstdint>
#include <string>

#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/BufferedClient.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace zappy::server::net::test {

namespace {

network::socket::Client makeClient(int fd, const std::string& ip, std::uint16_t port) {
    return network::socket::Client{fd, network::Address{ip, port}};
}

}  // namespace

namespace {

class SocketRegistryTest : public ::testing::Test {
  public:
    SocketRegistry registry;
};

}  // namespace

// ---------------------------------------------------------------------------
// findByAddress
// ---------------------------------------------------------------------------

TEST_F(SocketRegistryTest, GetFromAddressOnEmptyRegistryFails) {
    const network::Address addr{"127.0.0.1", 4242};

    ASSERT_EQ(registry.findByAddress(addr), nullptr);
}

TEST_F(SocketRegistryTest, GetFromAddressReturnsInsertedClient) {
    auto client = makeClient(10, "127.0.0.1", 4242);
    registry.insert(client);

    const network::Address addr{"127.0.0.1", 4242};
    const auto* result = registry.findByAddress(addr);

    ASSERT_EQ(result->fd(), 10);
}

TEST_F(SocketRegistryTest, GetFromAddressWithUnknownAddressFails) {
    auto client = makeClient(10, "127.0.0.1", 4242);
    registry.insert(client);

    const network::Address addr{"127.0.0.1", 9999};

    ASSERT_EQ(registry.findByAddress(addr), nullptr);
}

TEST_F(SocketRegistryTest, GetFromAddressDistinguishesByPort) {
    auto client = makeClient(10, "127.0.0.1", 1000);
    registry.insert(client);

    const network::Address samePortDifferentIp{"127.0.0.2", 1000};
    EXPECT_EQ(registry.findByAddress(samePortDifferentIp), nullptr);

    const network::Address differentPortSameIp{"127.0.0.1", 2000};
    EXPECT_EQ(registry.findByAddress(differentPortSameIp), nullptr);
}

TEST_F(SocketRegistryTest, GetFromAddressReturnsCorrectClientAmongMany) {
    auto first = makeClient(1, "127.0.0.1", 4000);
    auto second = makeClient(2, "127.0.0.2", 4001);
    auto third = makeClient(3, "127.0.0.3", 4002);
    registry.insert(first);
    registry.insert(second);
    registry.insert(third);

    const network::Address addr{"127.0.0.2", 4001};

    ASSERT_EQ(registry.findByAddress(addr)->fd(), 2);
}

// ---------------------------------------------------------------------------
// remove
// ---------------------------------------------------------------------------

TEST_F(SocketRegistryTest, RemoveDeletesClientByFd) {
    auto client = makeClient(10, "127.0.0.1", 4242);
    registry.insert(client);

    client = makeClient(30, "127.0.0.1", 4200);
    registry.insert(client);

    registry.remove(10);

    const network::Address invalidaddr{"127.0.0.1", 4242};
    const network::Address validaddr{"127.0.0.1", 4200};

    EXPECT_EQ(registry.findByAddress(invalidaddr), nullptr);
    EXPECT_NE(registry.findByAddress(validaddr), nullptr);
}

// Removing one client among many only affects the targeted fd.
TEST_F(SocketRegistryTest, RemoveOnlyAffectsTargetedClient) {
    auto first = makeClient(1, "127.0.0.1", 4000);
    auto second = makeClient(2, "127.0.0.2", 4001);
    registry.insert(first);
    registry.insert(second);

    registry.remove(1);

    const network::Address removedAddr{"127.0.0.1", 4000};
    EXPECT_EQ(registry.findByAddress(removedAddr), nullptr);

    const network::Address keptAddr{"127.0.0.2", 4001};
    ASSERT_EQ(registry.findByAddress(keptAddr)->fd(), 2);
}

// Removing on an empty registry must not crash.
TEST_F(SocketRegistryTest, RemoveOnEmptyRegistryIsSafe) { EXPECT_NO_THROW(registry.remove(42)); }

}  // namespace zappy::server::net::test
