/*
** EPITECH PROJECT, 2026
** ICommandHandler
** File description:
** ICommandHandler header
*/

#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/game/World.hpp"
#include "zappy/shared/io/Logger.hpp"

namespace zappy::server::command {

class ICommandGroup {
  public:
    ICommandGroup() = default;
    virtual ~ICommandGroup() = default;

    ICommandGroup(const ICommandGroup&) = delete;
    ICommandGroup(ICommandGroup&&) = delete;
    ICommandGroup& operator=(const ICommandGroup&) = delete;
    ICommandGroup& operator=(ICommandGroup&&) = delete;

    virtual void execute(Client* client, std::string_view cmd) = 0;

    virtual void operator()(Client* client, std::string_view cmd) = 0;

  protected:
    struct CommandData {
        std::string name;
        std::vector<std::string> params;
    };

    struct CommandCtx {
        std::reference_wrapper<Timer> timer;
        std::reference_wrapper<client::ClientRegistry> clientRegistry;
        std::reference_wrapper<game::World> world;
        std::reference_wrapper<io::Logger> logger;
        CommandData data{};
        Client* client = nullptr;
    };

    using CommandInvoker = std::function<bool(CommandCtx&)>;

    virtual CommandCtx& commandCtx() = 0;
    virtual CommandData extractCommand(std::string_view msg) = 0;
};

}  // namespace zappy::server::command
