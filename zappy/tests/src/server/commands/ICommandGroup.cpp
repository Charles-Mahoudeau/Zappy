/*
** EPITECH PROJECT, 2026
** ICommandGroup
** File description:
** ICommandGroup tests
*/

#include "zappy/server/commands/ICommandGroup.hpp"

#include <gtest/gtest.h>

#include <memory>
#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/net/SocketRegistry.hpp"
#include "zappy/shared/network/Address.hpp"

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
        const CommandData cmd = this->extractCommand(msg);

        if (cmd.name == "valid") {
            this->_update = true;
        }
        if (cmd.params.size() == 2 && cmd.params.at(0) == "valid2" && cmd.params.at(1) == "valid3") {
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
    zappy::server::net::SocketRegistry socketRegistry;
    std::unique_ptr<MockedCommand> mockCmd;
    std::unique_ptr<zappy::server::Client> client;

  protected:
    void SetUp() override {
        mockCmd = std::make_unique<MockedCommand>(timer, clients);
        client = std::make_unique<zappy::server::Client>(socketRegistry, zappy::network::Address{}, timer);
    }
};
}  // namespace

// ── NormalBehavior ────────────────────────────────────────────────────────────

TEST_F(TestICommandGroup, NormalBehavior) {
    mockCmd->execute(client.get(), "valid valid2 valid3");
    EXPECT_TRUE(mockCmd->validCmd());
    EXPECT_TRUE(mockCmd->validParam());
}

// ── Missing ────────────────────────────────────────────────────────────

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

// ── InvalidData ────────────────────────────────────────────────────────────

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
