/*
** EPITECH PROJECT, 2026
** ICommandHandler
** File description:
** ICommandHandler header
*/

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"

namespace zappy::server::command {

enum class CommandState : std::uint8_t {
    kSuccess,
    kInvalidCommand,
    kInvalidParam

};

class ICommandGroup {
  public:
    ICommandGroup() = default;
    virtual ~ICommandGroup() = default;

    ICommandGroup(const ICommandGroup&) = default;
    ICommandGroup(ICommandGroup&&) = delete;
    ICommandGroup& operator=(const ICommandGroup&) = default;
    ICommandGroup& operator=(ICommandGroup&&) = delete;

    virtual CommandState execute(Client* client, std::string_view cmd) = 0;

    virtual void operator()(Client* client, std::string_view cmd) = 0;

  protected:
    struct CommandData {
        std::string name;
        std::vector<std::string> params;
    };

    virtual Timer& timer() = 0;
    virtual client::ClientRegistry& clients() = 0;
    virtual CommandData extractCommand(std::string_view msg) = 0;
};

}  // namespace zappy::server::command
