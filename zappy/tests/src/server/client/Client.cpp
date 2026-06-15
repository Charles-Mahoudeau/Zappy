/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** server::Client tests
*/

#include "zappy/server/client/Client.hpp"

#include <fcntl.h>
#include <gtest/gtest.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <optional>
#include <string>

#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/socket/Client.hpp"

namespace {

struct SocketPair {
    int local = -1;
    int peer = -1;

    SocketPair() {
        std::array<int, 2> fds{};
        if (::socketpair(AF_UNIX, SOCK_STREAM, 0, fds.data()) == 0) {
            local = fds.at(0);
            peer = fds.at(1);
        }
    }
    ~SocketPair() {
        if (local != -1) {
            ::close(local);
        }
        if (peer != -1) {
            ::close(peer);
        }
    }

    SocketPair(const SocketPair&) = default;
    SocketPair(SocketPair&&) = delete;
    SocketPair& operator=(const SocketPair&) = default;
    SocketPair& operator=(SocketPair&&) = delete;
};

}  // namespace

namespace zappy::server::test {

class ServerClientTest : public ::testing::Test {
  protected:
    net::SocketRegistry registry;
    network::Address addr{"127.0.0.1", 4242};
};

// ---------------------------------------------------------------------------
// address / type / changeType
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, AddressReturnsConstructedAddress) {
    Client client{registry, addr};

    EXPECT_EQ(client.address(), addr);
}

TEST_F(ServerClientTest, TypeDefaultsToUnknown) {
    Client client{registry, addr};

    EXPECT_EQ(client.type(), Client::Type::kUNKNOWN);
}

TEST_F(ServerClientTest, ChangeTypeUpdatesType) {
    Client client{registry, addr};

    client.changeType(Client::Type::kPLAYER);
    EXPECT_EQ(client.type(), Client::Type::kPLAYER);

    client.changeType(Client::Type::kGUI);
    EXPECT_EQ(client.type(), Client::Type::kGUI);
}

// ---------------------------------------------------------------------------
// addRequest / getNextRequest
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, GetNextRequestOnEmptyReturnsNullopt) {
    Client client{registry, addr};

    EXPECT_FALSE(client.getNextRequest().has_value());
}

TEST_F(ServerClientTest, GetNextRequestReturnsRequestsInFifoOrder) {
    Client client{registry, addr};

    client.addRequest("first");
    client.addRequest("second");

    auto firstResult = client.getNextRequest();
    ASSERT_TRUE(firstResult.has_value());
    EXPECT_EQ(*firstResult, "first");

    auto secondResult = client.getNextRequest();
    ASSERT_TRUE(secondResult.has_value());
    EXPECT_EQ(*secondResult, "second");

    EXPECT_FALSE(client.getNextRequest().has_value());
}

TEST_F(ServerClientTest, AddRequestRespectsMaxRequestCap) {
    Client client{registry, addr};

    for (int i = 0; i < 12; ++i) {
        client.addRequest("req" + std::to_string(i));
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
    Client client{registry, addr};
    client.addRequest("delayed");

    client.setTimeout(2);

    EXPECT_FALSE(client.getNextRequest().has_value());
    EXPECT_FALSE(client.getNextRequest().has_value());

    auto result = client.getNextRequest();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "delayed");
}

TEST_F(ServerClientTest, ZeroTimeoutDoesNotBlock) {
    Client client{registry, addr};
    client.addRequest("immediate");

    client.setTimeout(0);

    auto result = client.getNextRequest();
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "immediate");
}

// ---------------------------------------------------------------------------
// update
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, UpdateReturnsFalseWhenSocketNotRegistered) {
    Client client{registry, addr};

    EXPECT_FALSE(client.update());
}

TEST_F(ServerClientTest, UpdateReturnsTrueWhenSocketRegistered) {
    const SocketPair pair;
    network::socket::Client socket{pair.local, addr};
    registry.insert(socket);

    Client client{registry, addr};

    const std::string msg = "ping\n";
    ASSERT_EQ(::write(pair.peer, msg.data(), msg.size()), static_cast<ssize_t>(msg.size()));

    EXPECT_TRUE(client.update());

    auto request = client.getNextRequest();
    ASSERT_TRUE(request.has_value());
    EXPECT_EQ(*request, "ping");
    ASSERT_FALSE(client.getNextRequest().has_value());
}

// ---------------------------------------------------------------------------
// sendMessage
// ---------------------------------------------------------------------------

TEST_F(ServerClientTest, SendMessageWithoutRegisteredSocketIsNoOp) {
    Client client{registry, addr};

    EXPECT_FALSE(client.sendMessage("hello"));
}

TEST_F(ServerClientTest, SendMessageWithRegisteredSocketDoesNotThrow) {
    const SocketPair pair;
    network::socket::Client socket{pair.local, addr};
    registry.insert(socket);

    Client client{registry, addr};

    EXPECT_TRUE(client.sendMessage("hello"));

    std::array<char, 32> buf{};
    const ssize_t n = ::read(pair.peer, buf.data(), buf.size());
    ASSERT_GT(n, 0);
    EXPECT_EQ(std::string(buf.data(), n), "hello");
}

}  // namespace zappy::server::test
