/*
** EPITECH PROJECT, 2026
** PlayerData
** File description:
** PlayerData code
*/

#include "zappy/server/commands/player/PlayerData.hpp"

#include <cstdint>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/World.hpp"
#include "zappy/server/game/entity/Player.hpp"
namespace zappy::server::command::player {

PlayerData::PlayerData(const ICommandGroup::CommandCtx& ctx, std::uint64_t id)
    : PlayerData(ctx.clientRegistry, ctx.world, id) {}

PlayerData::PlayerData(client::ClientRegistry& clients, game::World& world, std::uint64_t id)
    : _client(clients.findByPlayerId(id)) {
    if (this->_client == nullptr) {
        return;
    }
    this->_player = world.player(id);

    if (this->_player == nullptr) {
        this->_client->sendError();
    }
}

bool PlayerData::valid() const { return this->_player != nullptr && this->_client != nullptr; }

game::entity::Player* PlayerData::player() { return this->_player; }

const game::entity::Player* PlayerData::player() const { return this->_player; }

Client* PlayerData::client() { return this->_client; }

const Client* PlayerData::client() const { return this->_client; }

}  // namespace zappy::server::command::player
