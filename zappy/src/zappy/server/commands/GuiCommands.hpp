/*
** EPITECH PROJECT, 2026
** GuiCommands
** File description:
** GuiCommands header
*/

#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::command {
class GuiCommands : public ACommandGroup {
  public:
    explicit GuiCommands(CommandCtx context);
    ~GuiCommands() override = default;

    GuiCommands(const GuiCommands&) = delete;
    GuiCommands(GuiCommands&&) = delete;
    GuiCommands& operator=(const GuiCommands&) = delete;
    GuiCommands& operator=(GuiCommands&&) = delete;

    void execute(Client* client, std::string_view msg) override;

  private:
    static bool ignore(const CommandCtx& ctx);
    static bool msz(const CommandCtx& ctx);
    static bool bct(const CommandCtx& ctx);
    static bool mct(const CommandCtx& ctx);
    static bool tna(const CommandCtx& ctx);
    static bool ppo(const CommandCtx& ctx);
    static bool plv(const CommandCtx& ctx);
    static bool sgt(const CommandCtx& ctx);
    static bool sst(const CommandCtx& ctx);

    static std::optional<std::string> serializeTile(const CommandCtx& ctx, math::Vector2u position);

    static std::optional<std::uint32_t> parseUint32(const std::string& str);
    static std::optional<math::Vector2u> parsePosition(std::span<const std::string> params);

    std::unordered_map<std::string_view, CommandInvoker> _commands;
};
}  // namespace zappy::server::command
