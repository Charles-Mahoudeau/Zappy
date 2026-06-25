/*
** EPITECH PROJECT, 2026
** ICommandGroup
** File description:
** ICommandGroup tests
*/

#include "zappy/server/commands/ICommandGroup.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <optional>
#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/game/World.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/io/Logger.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace {

class MockedCommand : public zappy::server::command::ACommandGroup {
  public:
    using ACommandGroup::ACommandGroup;
    ~MockedCommand() override = default;

    MockedCommand(const MockedCommand&) = delete;
    MockedCommand(MockedCommand&&) = delete;
    MockedCommand& operator=(const MockedCommand&) = delete;
    MockedCommand& operator=(MockedCommand&&) = delete;

    void execute([[maybe_unused]] zappy::server::Client* client, std::string_view msg) override {
        const auto [name, params] = this->extractCommand(msg);

        if (name == "valid") {
            this->_update = true;
        }
        if (params.size() == 2 && params.at(0) == "valid2" && params.at(1) == "valid3") {
            this->_param = true;
        }
    }

    [[nodiscard]] bool validCmd() const { return this->_update; }
    [[nodiscard]] bool validParam() const { return this->_param; }

  private:
    bool _update = false;
    bool _param = false;
};

class TestICommandGroup : public ::testing::Test {
  public:
    zappy::server::Timer timer;
    zappy::server::client::ClientRegistry clients;
    zappy::server::client::TeamRegistry teams;
    zappy::server::net::SocketRegistry socketRegistry;
    std::unique_ptr<MockedCommand> mockCmd;
    std::unique_ptr<zappy::server::Client> client;
    std::unique_ptr<zappy::server::game::World> world;
    std::optional<zappy::io::Logger> logger;

  protected:
    void SetUp() override {
        logger.emplace("TestLogger");
        world = std::make_unique<zappy::server::game::World>(zappy::math::Vector2u{10, 10}, logger);
        mockCmd = std::make_unique<MockedCommand>(zappy::server::command::ICommandGroup::CommandCtx{
            .timer = timer,
            .clientRegistry = clients,
            .teamRegistry = teams,
            .world = *world,
            .logger = logger.value(),
        });
    }

    void TearDown() override {
        mockCmd.reset();
        world.reset();
        logger.reset();
    }
};

}  // namespace

// ── NormalBehavior ────────────────────────────────────────────────────────────

TEST_F(TestICommandGroup, NormalBehavior) {
    mockCmd->execute(client.get(), "valid valid2 valid3");
    EXPECT_TRUE(mockCmd->validCmd());
    EXPECT_TRUE(mockCmd->validParam());
}

// ── Missing ───────────────────────────────────────────────────────────────────

TEST_F(TestICommandGroup, emptyMsg) {
    mockCmd->execute(client.get(), "");
    EXPECT_FALSE(mockCmd->validCmd());
    EXPECT_FALSE(mockCmd->validParam());
}

TEST_F(TestICommandGroup, noParam) {
    mockCmd->execute(client.get(), "valid");
    EXPECT_TRUE(mockCmd->validCmd());
    EXPECT_FALSE(mockCmd->validParam());
}

// ── InvalidData ───────────────────────────────────────────────────────────────

TEST_F(TestICommandGroup, InvalidCmd) {
    mockCmd->execute(client.get(), "valid invalid");
    EXPECT_TRUE(mockCmd->validCmd());
    EXPECT_FALSE(mockCmd->validParam());
}

TEST_F(TestICommandGroup, invalidSecondParam) {
    mockCmd->execute(client.get(), "valid valid2 invalid");
    EXPECT_TRUE(mockCmd->validCmd());
    EXPECT_FALSE(mockCmd->validParam());
}
