/*
** EPITECH PROJECT, 2026
** ACommandGroup
** File description:
** ACommandGroup header
*/

#pragma once

#include <string_view>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/shared/network/Address.hpp"

namespace zappy::server::command {

class ACommandGroup : public ICommandGroup {
  public:
    explicit ACommandGroup(CommandCtx context);
    ~ACommandGroup() override = default;

    ACommandGroup(const ACommandGroup&) = delete;
    ACommandGroup(ACommandGroup&&) = delete;
    ACommandGroup& operator=(const ACommandGroup&) = delete;
    ACommandGroup& operator=(ACommandGroup&&) = delete;

    void operator()(Client* client, std::string_view cmd) override;

  protected:
    CommandCtx& commandCtx() override;
    CommandData extractCommand(std::string_view msg) override;
    static void trySendToAddress(CommandCtx& ctx, network::Address addr, std::string_view msg);

  private:
    CommandCtx _ctx;
};

}  // namespace zappy::server::command
