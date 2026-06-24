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
#include "zappy/server/game/World.hpp"
#include "zappy/shared/io/Logger.hpp"

namespace zappy::server::command {

class ACommandGroup : public ICommandGroup {
  public:
    ACommandGroup(Timer& timer, client::ClientRegistry& clients, game::World& world, io::Logger& logger);
    ~ACommandGroup() override = default;

    ACommandGroup(const ACommandGroup&) = delete;
    ACommandGroup(ACommandGroup&&) = delete;
    ACommandGroup& operator=(const ACommandGroup&) = delete;
    ACommandGroup& operator=(ACommandGroup&&) = delete;

    void operator()(Client* client, std::string_view cmd) override;

  protected:
    CommandCtx& commandCtx() override;
    CommandData extractCommand(std::string_view msg) override;

  private:
    CommandCtx _ctx;
};

}  // namespace zappy::server::command
