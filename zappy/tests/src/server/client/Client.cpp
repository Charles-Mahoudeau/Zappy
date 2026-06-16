/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** server::Client tests
*/

#include "zappy/shared/network/socket/Client.hpp"

#include <gtest/gtest.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <format>
#include <string>

#include "SocketPair.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server::test {

namespace {

class ServerClientTest : public ::testing::Test {
  public:
    net::SocketRegistry socketRegistry;
    network::Address addr{"127.0.0.1", 4242};
};

}  // namespace

// ---------------------------------------------------------------------------
// address / type / changeType
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, AddressReturnsConstructedAddress) {
    const Client client{socketRegistry, addr};

    EXPECT_EQ(client.address(), addr);
}

TEST_F(ServerClientTest, TypeDefaultsToUnknown) {
    const Client client{socketRegistry, addr};

    EXPECT_EQ(client.type(), Client::Type::kUNKNOWN);
}

TEST_F(ServerClientTest, ChangeTypeUpdatesType) {
    Client client{socketRegistry, addr};

    client.changeType(Client::Type::kPLAYER);
    EXPECT_EQ(client.type(), Client::Type::kPLAYER);

    client.changeType(Client::Type::kGUI);
    EXPECT_EQ(client.type(), Client::Type::kGUI);
}

// ---------------------------------------------------------------------------
// addRequest / getNextRequest
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, GetNextRequestOnEmptyReturnsNullopt) {
    Client client{socketRegistry, addr};

    EXPECT_FALSE(client.getNextRequest().has_value());
}

TEST_F(ServerClientTest, GetNextRequestReturnsRequestsInFifoOrder) {
    Client client{socketRegistry, addr};

    client.addRequest("first");
    client.addRequest("second");

    auto firstResult = client.getNextRequest();
    auto secondResult = client.getNextRequest();

    if (firstResult.has_value()) {
        EXPECT_EQ(*firstResult, "first");
    } else {
        FAIL() << "Expect first result but got nothing";
    }
    if (secondResult.has_value()) {
        EXPECT_EQ(*secondResult, "second");
    } else {
        FAIL() << "Expect second result but got nothing";
    }

    EXPECT_FALSE(client.getNextRequest().has_value());
}

TEST_F(ServerClientTest, AddRequestRespectsMaxRequestCap) {
    Client client{socketRegistry, addr};

    for (int i = 0; i < 12; ++i) {
        client.addRequest(std::format("req", std::to_string(i)));
    }

    int count = 0;
    while (auto request = client.getNextRequest()) {
        EXPECT_EQ(*request, "req" + std::to_string(count));
        ++count;
    }
    EXPECT_EQ(count, 10);
}

// ---------------------------------------------------------------------------
// setTimeout
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, GetNextRequestBlockedWhileTimeoutPending) {
    Client client{socketRegistry, addr};
    client.addRequest("delayed");

    client.setTimeout(2);

    if (client.getNextRequest().has_value()) {
        FAIL() << "Expect nothing on first request but got value";
    }
    if (client.getNextRequest().has_value()) {
        FAIL() << "Expect nothing on second request but got value";
    }

    auto result = client.getNextRequest();
    if (result.has_value()) {
        EXPECT_EQ(*result, "delayed");
    } else {
        FAIL() << "Expect result 'delayed' but got nothing";
    }
}

TEST_F(ServerClientTest, ZeroTimeoutDoesNotBlock) {
    Client client{socketRegistry, addr};
    client.addRequest("immediate");

    client.setTimeout(0);

    auto result = client.getNextRequest();
    if (result.has_value()) {
        EXPECT_EQ(*result, "immediate");
    } else {
        FAIL() << "Expect result but got nothing";
    }
}

// ---------------------------------------------------------------------------
// update
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, UpdateReturnsFalseWhenSocketNotRegistered) {
    Client client{socketRegistry, addr};

    EXPECT_FALSE(client.update());
}

TEST_F(ServerClientTest, UpdateReturnsTrueWhenSocketRegistered) {
    const SocketPair pair;
    network::socket::Client socket{pair.local, addr};
    socketRegistry.insert(socket);

    Client client{socketRegistry, addr};

    const std::string msg = "ping\n";
    ASSERT_EQ(::write(pair.peer, msg.data(), msg.size()), static_cast<ssize_t>(msg.size()));

    socketRegistry.getFromAddress(addr).value().get().poll();
    EXPECT_TRUE(client.update());

    if (auto request = client.getNextRequest(); request.has_value()) {
        EXPECT_EQ(*request, "ping");
    } else {
        FAIL() << "Expect request but got nothing";
    }
    ASSERT_FALSE(client.getNextRequest().has_value());
}

// ---------------------------------------------------------------------------
// sendMessage
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, SendMessageWithoutRegisteredSocketIsNoOp) {
    Client client{socketRegistry, addr};

    EXPECT_FALSE(client.sendMessage("hello"));
}

TEST_F(ServerClientTest, SendMessageWithRegisteredSocketDoesNotThrow) {
    const SocketPair pair;
    network::socket::Client socket{pair.local, addr};
    socketRegistry.insert(socket);

    Client client{socketRegistry, addr};

    EXPECT_TRUE(client.sendMessage("hello"));

    std::array<char, 32> buf{};
    const ssize_t n = ::read(pair.peer, buf.data(), buf.size());
    ASSERT_GT(n, 0);
    EXPECT_EQ(std::string(buf.data(), n), "hello");
}

}  // namespace zappy::server::test
