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
  protected:
    SocketRegistry registry;
};

}  // namespace

// ---------------------------------------------------------------------------
// getFromAddress
// ---------------------------------------------------------------------------

TEST_F(SocketRegistryTest, GetFromAddressOnEmptyRegistryFails) {
    network::Address addr{"127.0.0.1", 4242};

    auto result = registry.getFromAddress(addr);

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "Failed to get client Socket");
}

TEST_F(SocketRegistryTest, GetFromAddressReturnsInsertedClient) {
    auto client = makeClient(10, "127.0.0.1", 4242);
    registry.insert(client);

    network::Address addr{"127.0.0.1", 4242};
    auto result = registry.getFromAddress(addr);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->get().fd(), 10);
}

TEST_F(SocketRegistryTest, GetFromAddressWithUnknownAddressFails) {
    auto client = makeClient(10, "127.0.0.1", 4242);
    registry.insert(client);

    network::Address addr{"127.0.0.1", 9999};
    auto result = registry.getFromAddress(addr);

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(result.error(), "Failed to get client Socket");
}

TEST_F(SocketRegistryTest, GetFromAddressDistinguishesByPort) {
    auto client = makeClient(10, "192.168.1.1", 1000);
    registry.insert(client);

    network::Address samePortDifferentIp{"10.0.0.1", 1000};
    EXPECT_FALSE(registry.getFromAddress(samePortDifferentIp).has_value());

    network::Address differentPortSameIp{"192.168.1.1", 2000};
    EXPECT_FALSE(registry.getFromAddress(differentPortSameIp).has_value());
}

TEST_F(SocketRegistryTest, GetFromAddressReturnsCorrectClientAmongMany) {
    auto first = makeClient(1, "127.0.0.1", 4000);
    auto second = makeClient(2, "127.0.0.2", 4001);
    auto third = makeClient(3, "127.0.0.3", 4002);
    registry.insert(first);
    registry.insert(second);
    registry.insert(third);

    network::Address addr{"127.0.0.2", 4001};
    auto result = registry.getFromAddress(addr);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->get().fd(), 2);
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

    network::Address invalidaddr{"127.0.0.1", 4242};
    network::Address validaddr{"127.0.0.1", 4200};

    EXPECT_FALSE(registry.getFromAddress(invalidaddr).has_value());
    EXPECT_TRUE(registry.getFromAddress(validaddr).has_value());
}

// Removing one client among many only affects the targeted fd.
TEST_F(SocketRegistryTest, RemoveOnlyAffectsTargetedClient) {
    auto first = makeClient(1, "127.0.0.1", 4000);
    auto second = makeClient(2, "127.0.0.2", 4001);
    registry.insert(first);
    registry.insert(second);

    registry.remove(1);

    network::Address removedAddr{"127.0.0.1", 4000};
    EXPECT_FALSE(registry.getFromAddress(removedAddr).has_value());

    network::Address keptAddr{"127.0.0.2", 4001};
    auto kept = registry.getFromAddress(keptAddr);
    ASSERT_TRUE(kept.has_value());
    EXPECT_EQ(kept->get().fd(), 2);
}

// Removing on an empty registry must not crash.
TEST_F(SocketRegistryTest, RemoveOnEmptyRegistryIsSafe) { EXPECT_NO_THROW(registry.remove(42)); }

}  // namespace zappy::server::net::test
