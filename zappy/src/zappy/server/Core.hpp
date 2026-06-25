/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core header
*/

#pragma once

#include <memory>
#include <span>
#include <string_view>
#include <unordered_map>

#include "client/TeamRegistry.hpp"
#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/World.hpp"
#include "zappy/server/net/Server.hpp"
#include "zappy/shared/io/Logger.hpp"

namespace zappy::server {

class Core {
  public:
    Core() = default;
    ~Core() = default;

    Core(const Core&) = delete;
    Core(Core&&) = delete;
    Core& operator=(const Core&) = delete;
    Core& operator=(Core&&) = delete;

    void init(std::span<std::string_view> argv);

    [[noreturn]] void run();

    void processCommandGroup();

    void nextTick();

  private:
    [[nodiscard]] bool initTeams(std::span<const std::string_view> names);
    [[nodiscard]] bool initNetwork(std::uint16_t port);
    [[nodiscard]] bool initTimer(std::uint16_t frequency);
    [[nodiscard]] bool initWorld(math::Vector2u size);
    [[nodiscard]] bool initCommandGroups();

    io::Logger _logger{"Server", "server.log", true};
    net::Server _serv;
    Timer _timer;
    client::ClientRegistry _clientRegistry;
    std::unique_ptr<game::World> _world;
    std::unordered_map<Client::Type, std::unique_ptr<command::ICommandGroup>> _cmdGroups;
    client::TeamRegistry _teamRegistry;
};

}  // namespace zappy::server
