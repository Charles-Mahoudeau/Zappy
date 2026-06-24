/*
** EPITECH PROJECT, 2026
** ACommandGroup
** File description:
** ACommandGroup header
*/

#pragma once

#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"

namespace zappy::server::command {

class ACommandGroup : public ICommandGroup {
  public:
    ACommandGroup(Timer& timer, client::ClientRegistry& clients);
    ~ACommandGroup() override = default;

    ACommandGroup(const ACommandGroup&) = delete;
    ACommandGroup(ACommandGroup&&) = delete;
    ACommandGroup& operator=(const ACommandGroup&) = delete;
    ACommandGroup& operator=(ACommandGroup&&) = delete;

    void operator()(Client* client, std::string_view cmd) override;

  protected:
    Timer& timer() override;
    client::ClientRegistry& clients() override;
    CommandData extractCommand(std::string_view msg) override;

  private:
    Timer& _timer;
    client::ClientRegistry& _clients;
    // Members
};

}  // namespace zappy::server::command
