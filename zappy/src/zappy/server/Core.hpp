/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core header
*/

#pragma once

#include <span>
#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/net/Server.hpp"

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

    void run();

    void processCommands();

    void nextTick();

  private:
    net::Server _serv;
    Timer _time;
    client::ClientRegistry _clientRegistry;
};

}  // namespace zappy::server
