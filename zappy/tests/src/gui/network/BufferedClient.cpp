/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** BufferedClient tests
*/

#include "zappy/shared/network/BufferedClient.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>

#include "zappy/shared/exception/SocketError.hpp"
#include "zappy/shared/network/Address.hpp"
#include "zappy/shared/network/socket/Client.hpp"
#include "zappy/shared/network/socket/Server.hpp"

static constexpr uint16_t kTestPort = 59998;

class BufferedClientTest : public ::testing::Test {
  protected:
    void SetUp() override {
        _server.bind(kTestPort);
        _server.listen();
        _lineBuffer.connect(zappy::network::Address{"127.0.0.1", kTestPort});
        _serverClient.emplace(_server.accept());
    }

    void TearDown() override { _serverClient.reset(); }

    void serverSend(std::string_view data) {
        // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
        const auto* ptr = reinterpret_cast<const std::byte*>(data.data());
        std::size_t sent = 0;
        while (sent < data.size()) {
            sent += _serverClient->send(std::span<const std::byte>{ptr + sent, data.size() - sent});
        }
    }

    std::string serverRead() {
        auto bytes = _serverClient->read(4096);
        // NOLINTNEXTLINE(*-pro-type-reinterpret-cast)
        return {reinterpret_cast<const char*>(bytes.data()), bytes.size()};
    }

  public:
    zappy::network::socket::Server _server;
    std::optional<zappy::network::socket::Client> _serverClient;
    zappy::network::BufferedClient _lineBuffer;
};

TEST_F(BufferedClientTest, FdIsValidAfterConnect) { EXPECT_GT(_lineBuffer.fd(), 0); }

TEST_F(BufferedClientTest, HasNoMessagesInitially) { EXPECT_FALSE(_lineBuffer.hasMessages()); }

TEST_F(BufferedClientTest, PollExtractsCompleteLine) {
    serverSend("hello\n");
    _lineBuffer.poll();
    EXPECT_TRUE(_lineBuffer.hasMessages());
    EXPECT_EQ(_lineBuffer.popMessage(), "hello");
    EXPECT_FALSE(_lineBuffer.hasMessages());
}

TEST_F(BufferedClientTest, PollIgnoresPartialLine) {
    serverSend("hello");
    _lineBuffer.poll();
    EXPECT_FALSE(_lineBuffer.hasMessages());
}

TEST_F(BufferedClientTest, PollCompletesPartialLineAcrossTwoCalls) {
    serverSend("hel");
    _lineBuffer.poll();
    EXPECT_FALSE(_lineBuffer.hasMessages());
    serverSend("lo\n");
    _lineBuffer.poll();
    EXPECT_TRUE(_lineBuffer.hasMessages());
    EXPECT_EQ(_lineBuffer.popMessage(), "hello");
}

TEST_F(BufferedClientTest, PollExtractsMultipleLines) {
    serverSend("line1\nline2\n");
    _lineBuffer.poll();
    EXPECT_TRUE(_lineBuffer.hasMessages());
    EXPECT_EQ(_lineBuffer.popMessage(), "line1");
    EXPECT_TRUE(_lineBuffer.hasMessages());
    EXPECT_EQ(_lineBuffer.popMessage(), "line2");
    EXPECT_FALSE(_lineBuffer.hasMessages());
}

TEST_F(BufferedClientTest, PollExtractsCompleteLineAndKeepsPartial) {
    serverSend("complete\npartial");
    _lineBuffer.poll();
    EXPECT_TRUE(_lineBuffer.hasMessages());
    EXPECT_EQ(_lineBuffer.popMessage(), "complete");
    EXPECT_FALSE(_lineBuffer.hasMessages());
}

TEST_F(BufferedClientTest, PopMessageReturnsInFifoOrder) {
    serverSend("first\nsecond\nthird\n");
    _lineBuffer.poll();
    EXPECT_EQ(_lineBuffer.popMessage(), "first");
    EXPECT_EQ(_lineBuffer.popMessage(), "second");
    EXPECT_EQ(_lineBuffer.popMessage(), "third");
}

TEST_F(BufferedClientTest, SendDataReachesServer) {
    _lineBuffer.send("GRAPHIC\n");
    EXPECT_EQ(serverRead(), "GRAPHIC\n");
}

TEST_F(BufferedClientTest, SendMultipleLinesReachServer) {
    _lineBuffer.send("msz\n");
    _lineBuffer.send("mct\n");
    const std::string received = serverRead();
    EXPECT_EQ(received, "msz\nmct\n");
}

TEST_F(BufferedClientTest, PollThrowsWhenConnectionLost) {
    _serverClient.reset();
    EXPECT_THROW(_lineBuffer.poll(), zappy::exception::SocketError);
}
