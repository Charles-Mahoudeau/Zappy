/*
** EPITECH PROJECT, 2026
** GuiCommands
** File description:
** GuiCommands code
*/

#include "zappy/server/commands/GuiCommands.hpp"

#include <cstdint>
#include <format>
#include <limits>
#include <optional>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <utility>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ACommandGroup.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/Tile.hpp"
#include "zappy/server/game/WireHelper.hpp"
#include "zappy/server/game/entity/Player.hpp"
#include "zappy/shared/exception/OutOfRange.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server::command {
GuiCommands::GuiCommands(CommandCtx context)
    : ACommandGroup{std::move(context)},
      _commands({
          {"msz", [](const CommandCtx& ctx) { return msz(ctx); }},
          {"bct", [](const CommandCtx& ctx) { return bct(ctx); }},
          {"mct", [](const CommandCtx& ctx) { return mct(ctx); }},
          {"tna", [](const CommandCtx& ctx) { return tna(ctx); }},
          {"ppo", [](const CommandCtx& ctx) { return ppo(ctx); }},
          {"plv", [](const CommandCtx& ctx) { return plv(ctx); }},
          {"pin", [](const CommandCtx& ctx) { return pin(ctx); }},
          {"sgt", [](const CommandCtx& ctx) { return sgt(ctx); }},
          {"sst", [](const CommandCtx& ctx) { return sst(ctx); }},
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

    try {
        const game::Tile& tile = ctx.world.get().grid().tile(*position);

        std::ignore = ctx.client->sendMessage(game::WireHelper::tileToBctCommand(tile));
        return true;
    } catch (const exception::OutOfRange& e) {
        ctx.logger.get().warn(std::format("tile out of range: {}", e.what()));
    }
    return false;
}

bool GuiCommands::mct(const CommandCtx& ctx) {
    std::ignore = ctx.client->sendMessage(game::WireHelper::worldToBctCommands(ctx.world));
    return true;
}

bool GuiCommands::tna(const CommandCtx& ctx) {
    std::stringstream stringStream;

    for (const std::string& teamName : ctx.teamRegistry.get().teamNames()) {
        stringStream << std::format("tna {}\n", teamName);
    }
    std::ignore = ctx.client->sendMessage(stringStream.str());
    return true;
}

bool GuiCommands::ppo(const CommandCtx& ctx) {
    const std::optional<std::uint32_t> playerId = parsePlayerId(ctx.data.params.at(0));

    if (!playerId.has_value()) {
        ctx.logger.get().warn("invalid argument");
        return false;
    }

    const game::entity::Player* player = ctx.world.get().player(*playerId);

    if (player == nullptr) {
        ctx.logger.get().warn("player not found");
        return false;
    }

    const math::Vector2u playerPosition = player->position();

    std::ignore = ctx.client->sendMessage(std::format("ppo #{} {} {} {}\n", *playerId, playerPosition.x,
                                                      playerPosition.y, std::to_underlying(player->orientation())));
    return true;
}

bool GuiCommands::plv(const CommandCtx& ctx) {
    const std::optional<std::uint32_t> playerId = parsePlayerId(ctx.data.params.at(0));

    if (!playerId.has_value()) {
        ctx.logger.get().warn("invalid argument");
        return false;
    }

    const game::entity::Player* player = ctx.world.get().player(*playerId);

    if (player == nullptr) {
        ctx.logger.get().warn("player not found");
        return false;
    }
    std::ignore = ctx.client->sendMessage(std::format("plv #{} {}\n", *playerId, player->level()));
    return true;
}

bool GuiCommands::pin(const CommandCtx& ctx) {
    const std::optional<std::uint32_t> playerId = parsePlayerId(ctx.data.params.at(0));

    if (!playerId.has_value()) {
        ctx.logger.get().warn("invalid argument");
        return false;
    }

    const game::entity::Player* player = ctx.world.get().player(*playerId);

    if (player == nullptr) {
        ctx.logger.get().warn("player not found");
        return false;
    }

    math::Vector2u position = player->position();

    std::ignore = ctx.client->sendMessage(
        std::format("pin #{} {} {} {}\n", *playerId, position.x, position.y, player->inventory().string()));
    return true;
}

bool GuiCommands::sgt(const CommandCtx& ctx) {
    std::ignore = ctx.client->sendMessage(std::format("sgt {}\n", ctx.timer.get().frequency()));
    return true;
}

bool GuiCommands::sst(const CommandCtx& ctx) {
    if (ctx.data.params.size() != 1) {
        ctx.logger.get().warn("invalid number of arguments");
        return false;
    }
    const std::optional<std::uint32_t> timeUnit = parseUint32(ctx.data.params.at(0));

    if (!timeUnit.has_value()) {
        ctx.logger.get().warn("invalid argument");
        return false;
    }
    ctx.timer.get().setFrequency(*timeUnit);
    std::ignore = ctx.clientRegistry.get().broadcast(Client::Type::kGui, "sgt {}\n", ctx.timer.get().frequency());
    ctx.logger.get().info("timer frequency set to {} (from '{}')", ctx.timer.get().frequency(), ctx.data.params.at(0));
    return true;
}

std::optional<std::uint32_t> GuiCommands::parseUint32(const std::string& str) {
    try {
        const std::uint64_t v = std::stoul(str);

        if (v > std::numeric_limits<std::uint32_t>::max()) {
            return std::nullopt;
        }
        return v;
    } catch (const std::invalid_argument&) {
        return std::nullopt;
    } catch (const std::out_of_range&) {
        return std::nullopt;
    }
}

std::optional<math::Vector2u> GuiCommands::parsePosition(const std::span<const std::string> params) {
    if (params.size() != 2) {
        return std::nullopt;
    }

    try {
        // NOLINTBEGIN(*-pro-bounds-avoid-unchecked-container-access)
        const std::uint64_t x = std::stoul(params[0]);
        const std::uint64_t y = std::stoul(params[1]);
        // NOLINTEND(*-pro-bounds-avoid-unchecked-container-access)

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

std::optional<std::uint32_t> GuiCommands::parsePlayerId(const std::string_view str) {
    if (!str.starts_with('#')) {
        return std::nullopt;
    }
    return parseUint32(std::string{str.substr(1)});
}
}  // namespace zappy::server::command
