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
#include <chrono>
#include <format>
#include <string>
#include <thread>

#include "SocketPair.hpp"
#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server::test {

namespace {

class ServerClientTest : public ::testing::Test {
  public:
    net::SocketRegistry socketRegistry;
    network::Address addr{"127.0.0.1", 4242};
    Timer timer;
};  // namespace

}  // namespace

// ---------------------------------------------------------------------------
// address / type / changeType
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, AddressReturnsConstructedAddress) {
    const Client client{socketRegistry, addr, timer};

    EXPECT_EQ(client.address(), addr);
}

TEST_F(ServerClientTest, TypeDefaultsToUnknown) {
    const Client client{socketRegistry, addr, timer};

    EXPECT_EQ(client.type(), Client::Type::kUnknown);
}

TEST_F(ServerClientTest, ChangeTypeUpdatesType) {
    Client client{socketRegistry, addr, timer};

    client.changeType(Client::Type::kPlayer);
    EXPECT_EQ(client.type(), Client::Type::kPlayer);

    client.changeType(Client::Type::kGui);
    EXPECT_EQ(client.type(), Client::Type::kGui);
}

// ---------------------------------------------------------------------------
// addRequest / nextRequest
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, GetNextRequestOnEmptyReturnsNullopt) {
    Client client{socketRegistry, addr, timer};

    EXPECT_FALSE(client.nextRequest().has_value());
}

TEST_F(ServerClientTest, GetNextRequestReturnsRequestsInFifoOrder) {
    Client client{socketRegistry, addr, timer};

    client.addRequest("first");
    client.addRequest("second");

    auto firstResult = client.nextRequest();
    auto secondResult = client.nextRequest();

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

    EXPECT_FALSE(client.nextRequest().has_value());
}

TEST_F(ServerClientTest, AddRequestRespectsMaxRequestCap) {
    Client client{socketRegistry, addr, timer};

    for (int i = 0; i < 12; ++i) {
        client.addRequest(std::format("req{}", std::to_string(i)));
    }

    int count = 0;
    while (auto request = client.nextRequest()) {
        EXPECT_EQ(*request, "req" + std::to_string(count));
        ++count;
    }
    EXPECT_EQ(count, 10);
}

// ---------------------------------------------------------------------------
// setTimeout
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, GetNextRequestBlockedWhileTimeoutPending) {
    Client client{socketRegistry, addr, timer};
    client.addRequest("delayed");

    timer.setFrequencies(50);

    client.setTimeout(3);

    for (int i = 0; i < 3; i++) {
        ASSERT_FALSE(client.nextRequest().has_value()) << "Expect not nothing but got something at " << i << " loop";

        int const wait = timer.timeoutUntilNextTick();
        std::this_thread::sleep_for(std::chrono::milliseconds(wait + 1));
        timer.update();
    }

    auto result = client.nextRequest();
    if (result.has_value()) {
        EXPECT_EQ(*result, "delayed");
    } else {
        FAIL() << "Expect result 'delayed' but got nothing";
    }
}

TEST_F(ServerClientTest, ZeroTimeoutDoesNotBlock) {
    Client client{socketRegistry, addr, timer};
    client.addRequest("immediate");

    client.setTimeout(0);

    auto result = client.nextRequest();
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
    Client client{socketRegistry, addr, timer};

    EXPECT_FALSE(client.update());
}

TEST_F(ServerClientTest, UpdateReturnsTrueWhenSocketRegistered) {
    const SocketPair pair;
    network::socket::Client socket{pair.local, addr};
    socketRegistry.insert(socket);

    Client client{socketRegistry, addr, timer};

    const std::string msg = "ping\n";
    ASSERT_EQ(::write(pair.peer, msg.data(), msg.size()), static_cast<ssize_t>(msg.size()));

    socketRegistry.findByAddress(addr)->poll();
    EXPECT_TRUE(client.update());

    if (auto request = client.nextRequest(); request.has_value()) {
        EXPECT_EQ(*request, "ping");
    } else {
        FAIL() << "Expect request but got nothing";
    }
    ASSERT_FALSE(client.nextRequest().has_value());
}

// ---------------------------------------------------------------------------
// sendMessage
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, SendMessageWithoutRegisteredSocketIsNoOp) {
    const Client client{socketRegistry, addr, timer};

    EXPECT_FALSE(client.sendMessage("hello"));
}

TEST_F(ServerClientTest, SendMessageWithRegisteredSocketDoesNotThrow) {
    const SocketPair pair;
    network::socket::Client socket{pair.local, addr};
    socketRegistry.insert(socket);

    const Client client{socketRegistry, addr, timer};

    ASSERT_TRUE(client.sendMessage("hello"));

    std::array<char, 32> buf{};
    const ssize_t n = ::read(pair.peer, buf.data(), buf.size());
    ASSERT_GT(n, 0);
    EXPECT_EQ(std::string(buf.data(), n), "hello");
}

TEST_F(ServerClientTest, setTimeoutTest) {
    Client client{socketRegistry, addr, timer};

    timer.setFrequencies(100);

    EXPECT_FALSE(client.inTimeout());
    client.setTimeout(1);
    EXPECT_TRUE(client.inTimeout());
    std::this_thread::sleep_for(std::chrono::milliseconds(timer.timeoutUntilNextTick() + 1));
    timer.update();
    EXPECT_FALSE(client.inTimeout());
}

TEST_F(ServerClientTest, removeTimeout) {
    Client client{socketRegistry, addr, timer};

    timer.setFrequencies(10);

    EXPECT_FALSE(client.inTimeout());
    client.setTimeout(1);
    EXPECT_TRUE(client.inTimeout());
    client.removeTimeout();
    EXPECT_FALSE(client.inTimeout());
}

}  // namespace zappy::server::test
