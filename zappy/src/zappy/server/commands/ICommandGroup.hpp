/*
** EPITECH PROJECT, 2026
** ICommandHandler
** File description:
** ICommandHandler header
*/

#pragma once

#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
namespace zappy::server::command {

class ICommandGroup {
  public:
    ICommandGroup() = default;
    virtual ~ICommandGroup() = default;

    ICommandGroup(const ICommandGroup&) = default;
    ICommandGroup(ICommandGroup&&) = delete;
    ICommandGroup& operator=(const ICommandGroup&) = default;
    ICommandGroup& operator=(ICommandGroup&&) = delete;

    virtual void execute(Client* client, std::string_view cmd) = 0;

    virtual void operator()(Client* client, std::string_view cmd) = 0;

  protected:
    virtual Timer& timer() = 0;
    virtual client::ClientRegistry& clients() = 0;
};

}  // namespace zappy::server::command
