/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** BufferedClient tests
*/

#include "zappy/shared/network/BufferedClient.hpp"

#include <gtest/gtest.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/socket/Client.hpp"
#include "zappy/shared/network/socket/Server.hpp"

namespace {

constexpr uint16_t kTestPort = 59998;

class BufferedClientTest : public ::testing::Test {
  protected:
    void SetUp() override {
        _server.bind(kTestPort);
        _server.listen();
        _bufferedClient.connect(zappy::network::Address{"127.0.0.1", kTestPort});
        _serverClient.emplace(_server.accept());
    }

    void TearDown() override { _serverClient.reset(); }

    static void serverSend(zappy::network::socket::Client& client, std::string_view data) {
        auto bytes = std::as_bytes(std::span{data});
        if (bytes.empty()) {
            return;
        }
        std::size_t sent = 0;
        while (sent < bytes.size()) {
            sent += client.send(bytes.subspan(sent));
        }
    }

    static std::string serverRead(zappy::network::socket::Client& client) {
        auto bytes = client.read(4096);
        if (bytes.empty()) {
            return {};
        }
        std::string result(bytes.size(), '\0');
        std::ranges::transform(bytes, result.begin(),
                               [](std::byte b) { return static_cast<char>(std::to_underlying(b)); });
        return result;
    }

  public:
    zappy::network::socket::Server _server;
    std::optional<zappy::network::socket::Client> _serverClient;
    zappy::network::BufferedClient _bufferedClient;
};

TEST_F(BufferedClientTest, FdIsValidAfterConnect) { EXPECT_GT(_bufferedClient.fd(), 0); }

TEST_F(BufferedClientTest, HasNoMessagesInitially) { EXPECT_FALSE(_bufferedClient.hasMessages()); }

TEST_F(BufferedClientTest, PollExtractsCompleteLine) {
    serverSend(_serverClient.value(), "hello\n");
    _bufferedClient.poll();
    EXPECT_TRUE(_bufferedClient.hasMessages());
    EXPECT_EQ(_bufferedClient.popMessage(), "hello");
    EXPECT_FALSE(_bufferedClient.hasMessages());
}

TEST_F(BufferedClientTest, PollIgnoresPartialLine) {
    serverSend(_serverClient.value(), "hello");
    _bufferedClient.poll();
    EXPECT_FALSE(_bufferedClient.hasMessages());
}

TEST_F(BufferedClientTest, PollCompletesPartialLineAcrossTwoCalls) {
    serverSend(_serverClient.value(), "hel");
    _bufferedClient.poll();
    EXPECT_FALSE(_bufferedClient.hasMessages());
    serverSend(_serverClient.value(), "lo\n");
    _bufferedClient.poll();
    EXPECT_TRUE(_bufferedClient.hasMessages());
    EXPECT_EQ(_bufferedClient.popMessage(), "hello");
}

TEST_F(BufferedClientTest, PollExtractsMultipleLines) {
    serverSend(_serverClient.value(), "line1\nline2\n");
    _bufferedClient.poll();
    EXPECT_TRUE(_bufferedClient.hasMessages());
    EXPECT_EQ(_bufferedClient.popMessage(), "line1");
    EXPECT_TRUE(_bufferedClient.hasMessages());
    EXPECT_EQ(_bufferedClient.popMessage(), "line2");
    EXPECT_FALSE(_bufferedClient.hasMessages());
}

TEST_F(BufferedClientTest, PollExtractsCompleteLineAndKeepsPartial) {
    serverSend(_serverClient.value(), "complete\npartial");
    _bufferedClient.poll();
    EXPECT_TRUE(_bufferedClient.hasMessages());
    EXPECT_EQ(_bufferedClient.popMessage(), "complete");
    EXPECT_FALSE(_bufferedClient.hasMessages());
}

TEST_F(BufferedClientTest, PopMessageReturnsInFifoOrder) {
    serverSend(_serverClient.value(), "first\nsecond\nthird\n");
    _bufferedClient.poll();
    EXPECT_EQ(_bufferedClient.popMessage(), "first");
    EXPECT_EQ(_bufferedClient.popMessage(), "second");
    EXPECT_EQ(_bufferedClient.popMessage(), "third");
}

TEST_F(BufferedClientTest, SendDataReachesServer) {
    _bufferedClient.send("GRAPHIC\n");
    EXPECT_EQ(serverRead(_serverClient.value()), "GRAPHIC\n");
}

TEST_F(BufferedClientTest, SendMultipleLinesReachServer) {
    _bufferedClient.send("msz\n");
    _bufferedClient.send("mct\n");
    const std::string received = serverRead(_serverClient.value());
    EXPECT_EQ(received, "msz\nmct\n");
}

TEST_F(BufferedClientTest, PollThrowsWhenConnectionLost) {
    _serverClient.reset();
    EXPECT_THROW(_bufferedClient.poll(), zappy::exception::SocketError);
}

}  // namespace
