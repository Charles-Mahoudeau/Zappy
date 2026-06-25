/*
** EPITECH PROJECT, 2026
** GuiCommands
** File description:
** GuiCommands code
*/

#include "zappy/server/commands/GuiCommands.hpp"

#include <string_view>
#include <tuple>
#include <utility>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/shared/exception/OutOfRange.hpp"

namespace zappy::server::command {
GuiCommands::GuiCommands(CommandCtx context)
    : ACommandGroup{std::move(context)},
      _commands({
          {"msz", [](const CommandCtx& ctx) { return msz(ctx); }},
          {"bct", [](auto& ctx) { return bct(ctx); }},
          {"mct", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"tna", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"ppo", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"plv", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"pin", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"sgt", [](auto& ctx) { return GuiCommands::ignore(ctx); }},
          {"sst", [](auto& ctx) { return GuiCommands::ignore(ctx); }},

      }) {}

void GuiCommands::execute(Client* client, [[maybe_unused]] const std::string_view msg) {
    CommandCtx& ctx = this->commandCtx();

    if (ctx.client == nullptr) {
        ctx.logger.get().warn("ctx.client == nullptr, are you using execute instead of operator()?");
        if (client != nullptr) {
            std::ignore = client->sendMessage("suc\n");
        }
        return;
    }
    if (client != ctx.client) {
        ctx.logger.get().warn("client != ctx.client, context may be stale.");
    }

    const auto it = _commands.find(ctx.data.name);

    if (it == _commands.end()) {
        std::ignore = ctx.client->sendMessage("suc\n");
        return;
    }
    if (!it->second(ctx)) {
        std::ignore = ctx.client->sendMessage("sbp\n");
    }
}

bool GuiCommands::ignore(const CommandCtx& ctx) {
    (void)ctx;
    return false;
}

bool GuiCommands::msz(const CommandCtx& ctx) {
    math::Vector2u worldSize = ctx.world.get().size();

    std::ignore = ctx.client->sendMessage(std::format("msz {} {}\n", worldSize.x, worldSize.y));
    return true;
}

bool GuiCommands::bct(const CommandCtx& ctx) {
    const std::optional<math::Vector2u> position = parsePosition(ctx.data.params);

    if (!position.has_value()) {
        ctx.logger.get().warn("bct invalid position");
        return false;
    }

    const std::optional<std::string> serializedTile = serializeTile(ctx, *position);

    if (!serializedTile.has_value()) {
        return false;
    }
    std::ignore = ctx.client->sendMessage(*serializedTile);
    return true;
}

std::optional<std::string> GuiCommands::serializeTile(const CommandCtx& ctx, math::Vector2u position) {
    try {
        const game::Tile& tile = ctx.world.get().grid().tile(position);

        return std::format("bct {} {} {}\n", position.x, position.y, tile.inventory().string());
    } catch (const exception::OutOfRange& e) {
        ctx.logger.get().warn(std::format("tile out of range: {}", e.what()));
    }
    return std::nullopt;
}

std::optional<math::Vector2u> GuiCommands::parsePosition(const std::span<const std::string> params) {
    if (params.size() != 2) {
        return std::nullopt;
    }

    try {
        const std::uint64_t x = std::stoul(params[0]);
        const std::uint64_t y = std::stoul(params[1]);

        if (x > std::numeric_limits<std::uint32_t>::max() || y > std::numeric_limits<std::uint32_t>::max()) {
            return std::nullopt;
        }
        return math::Vector2u{
            static_cast<std::uint32_t>(x),
            static_cast<std::uint32_t>(y),
        };
    } catch (const std::invalid_argument&) {
        return std::nullopt;
    } catch (const std::out_of_range&) {
        return std::nullopt;
    }
}
}  // namespace zappy::server::command
