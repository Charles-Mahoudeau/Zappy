/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GUI network functional tests
*/

#include <gtest/gtest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <thread>
#include <utility>

#include "zappy/gui/GUI.hpp"
#include "zappy/gui/GuiCliParser.hpp"
#include "zappy/gui/game/GameState.hpp"
#include "zappy/shared/exception/InvalidState.hpp"
#include "zappy/shared/network/socket/Client.hpp"
#include "zappy/shared/network/socket/Server.hpp"

namespace {

using zappy::gui::GUI;
using zappy::gui::GuiCliParser;
using zappy::gui::game::Orientation;

constexpr uint16_t kTestPort = 59997;
constexpr std::array<std::string_view, 4> kCliArgs{"-h", "127.0.0.1", "-p", "59997"};

class GuiFunctionalTest : public ::testing::Test {
  public:
    void SetUp() override {
        _server.bind(kTestPort);
        _server.listen();
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

    std::string serverReadLines(zappy::network::socket::Client& client, std::size_t expectedLines) {
        while (static_cast<std::size_t>(std::ranges::count(_leftover, '\n')) < expectedLines) {
            _leftover += serverRead(client);
        }
        std::size_t pos = 0;
        for (std::size_t i = 0; i < expectedLines; ++i) {
            pos = _leftover.find('\n', pos) + 1;
        }
        std::string result = _leftover.substr(0, pos);
        _leftover.erase(0, pos);
        return result;
    }

    void connectGui() {
        const GuiCliParser cli{std::span{kCliArgs}};
        std::thread clientThread([this, &cli]() { _gui.connect(cli); });

        _serverClient = std::make_unique<zappy::network::socket::Client>(_server.accept());

        serverSend(*_serverClient, "WELCOME\n");
        EXPECT_EQ(serverReadLines(*_serverClient, 1), "GRAPHIC\n");
        EXPECT_EQ(serverReadLines(*_serverClient, 4), "msz\nmct\ntna\nsgt\n");
        serverSend(*_serverClient,
                   "msz 2 1\n"
                   "bct 0 0 0 0 0 0 0 0 0\n"
                   "bct 1 0 0 0 0 0 0 0 0\n"
                   "tna TeamA\n"
                   "tna TeamB\n"
                   "sgt 100\n");

        clientThread.join();
    }
    zappy::network::socket::Server _server;
    std::unique_ptr<zappy::network::socket::Client> _serverClient;
    GUI _gui;

  private:
    std::string _leftover;
};

// Scenario 1: full handshake driven through GUI::connect() over a real socket.
TEST_F(GuiFunctionalTest, ConnectCompletesHandshakeAgainstRealServer) {
    connectGui();

    EXPECT_TRUE(_gui.state().isReady());
    EXPECT_EQ(_gui.state().width(), 2U);
    EXPECT_EQ(_gui.state().height(), 1U);
    ASSERT_EQ(_gui.state().teams().size(), 2U);
    EXPECT_EQ(_gui.state().teams().at(0), "TeamA");
    EXPECT_EQ(_gui.state().timeUnit(), 100U);
}

TEST_F(GuiFunctionalTest, ConnectThrowsWhenFirstMessageIsNotWelcome) {
    const GuiCliParser cli{std::span{kCliArgs}};
    std::thread clientThread([this, &cli]() { EXPECT_THROW(_gui.connect(cli), zappy::exception::InvalidState); });

    _serverClient = std::make_unique<zappy::network::socket::Client>(_server.accept());
    serverSend(*_serverClient, "NOTWELCOME\n");

    clientThread.join();
}

// Scenario 2: post-handshake protocol batch dispatched through GUI::pump().
TEST_F(GuiFunctionalTest, FullProtocolBatchUpdatesGameStateAcrossTheWire) {
    connectGui();

    serverSend(*_serverClient,
               "pnw #1 0 0 2 1 TeamA\n"
               "ppo #1 1 0 3\n"
               "plv #1 2\n"
               "pin #1 1 0 1 0 0 0 0 0 0\n"
               "pnw #2 1 1 1 1 TeamB\n"
               "pex #2\n"
               "pbc #1 hello world\n"
               "pic 1 0 2 #1\n"
               "pie 1 0 1\n"
               "pfk #1\n"
               "pdr #1 0\n"
               "pgt #1 0\n"
               "enw #10 #1 0 0\n"
               "ebo #10\n"
               "enw #11 #2 1 0\n"
               "edi #11\n"
               "sgt 150\n"
               "sst 150\n"
               "seg TeamA\n"
               "smg server message\n"
               "suc\n"
               "sbp\n"
               "bct 1 0 0 2 0 0 0 0 0\n");
    _gui.pump();

    // pnw / ppo / plv / pin
    ASSERT_EQ(_gui.state().players().count(1), 1U);
    const auto& player1 = _gui.state().players().at(1);
    EXPECT_EQ(player1.x, 1U);
    EXPECT_EQ(player1.y, 0U);
    EXPECT_EQ(player1.orientation, Orientation::South);
    EXPECT_EQ(player1.level, 2U);
    EXPECT_EQ(player1.team, "TeamA");
    EXPECT_EQ(player1.inventory.food, 1U);

    // pic sets incanting true, pie (matching x/y) clears it again.
    EXPECT_FALSE(player1.isIncanting);

    // pbc -> broadcast recorded, and smg -> also recorded as a
    // broadcast-like message, appended after pbc's "hello world".
    ASSERT_FALSE(_gui.state().broadcasts().empty());
    EXPECT_EQ(_gui.state().broadcasts().back(), "server message");
    EXPECT_TRUE(
        std::ranges::any_of(_gui.state().broadcasts(), [](const std::string& msg) { return msg == "hello world"; }));

    // enw / ebo / edi -> egg #10 removed, egg #11 added then removed.
    EXPECT_EQ(_gui.state().eggs().count(10), 0U);
    EXPECT_EQ(_gui.state().eggs().count(11), 0U);

    // sgt / sst -> time unit updated (sst applied last with same value).
    EXPECT_EQ(_gui.state().timeUnit(), 150U);

    // seg -> winner set, game over.
    ASSERT_TRUE(_gui.state().winner().has_value());
    EXPECT_EQ(_gui.state().winner().value(), "TeamA");
    EXPECT_TRUE(_gui.state().isGameOver());

    // bct -> tile resources updated for a tile distinct from the initial one.
    EXPECT_EQ(_gui.state().tile(1, 0).linemate, 2U);

    // pex / pfk / pdr / pgt / suc / sbp are no-ops on GameState but must not
    // throw and must not have corrupted player #2, which never received a
    // position update.
    ASSERT_EQ(_gui.state().players().count(2), 1U);
    EXPECT_EQ(_gui.state().players().at(2).team, "TeamB");
}

TEST_F(GuiFunctionalTest, PlayerDeathRemovesPlayerAfterWireDispatch) {
    connectGui();

    serverSend(*_serverClient, "pnw #5 0 0 1 1 TeamA\n");
    _gui.pump();
    ASSERT_EQ(_gui.state().players().count(5), 1U);

    serverSend(*_serverClient, "pdi #5\n");
    _gui.pump();

    EXPECT_EQ(_gui.state().players().count(5), 0U);
}

TEST_F(GuiFunctionalTest, EggLifecycleAcrossTheWire) {
    connectGui();

    serverSend(*_serverClient, "pnw #1 0 0 1 1 TeamA\nenw #7 #1 0 0\n");
    _gui.pump();

    ASSERT_EQ(_gui.state().eggs().count(7), 1U);
    EXPECT_EQ(_gui.state().eggs().at(7).x, 0U);
    EXPECT_EQ(_gui.state().eggs().at(7).y, 0U);
    EXPECT_EQ(_gui.state().eggs().at(7).playerId, 1U);

    serverSend(*_serverClient, "ebo #7\n");
    _gui.pump();

    EXPECT_EQ(_gui.state().eggs().count(7), 0U);
}

// Scenario 3: a multi-line TCP packet split mid-line still parses correctly
// once reassembled by BufferedClient, then dispatched as full game commands.
TEST_F(GuiFunctionalTest, PartialTcpFragmentsAreReassembledBeforeDispatch) {
    connectGui();

    serverSend(*_serverClient, "pnw #9 1 1 1 1 Team");
    _gui.pump();
    EXPECT_EQ(_gui.state().players().count(9), 0U);

    serverSend(*_serverClient, "X\nppo #9 0 0 2\n");
    _gui.pump();

    ASSERT_EQ(_gui.state().players().count(9), 1U);
    const auto& player = _gui.state().players().at(9);
    EXPECT_EQ(player.team, "TeamX");
    EXPECT_EQ(player.x, 0U);
    EXPECT_EQ(player.y, 0U);
    EXPECT_EQ(player.orientation, Orientation::East);
}

}  // namespace
