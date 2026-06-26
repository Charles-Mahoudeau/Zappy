/*
** EPITECH PROJECT, 2026
** PlayerData
** File description:
** PlayerData code
*/

#include "zappy/server/commands/player/PlayerData.hpp"

#include <cstdint>

#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/game/IEntity.hpp"
#include "zappy/server/game/entity/Player.hpp"
namespace zappy::server::command::player {

PlayerData::PlayerData(ICommandGroup::CommandCtx& ctx, std::uint64_t id)
    : _client(ctx.clientRegistry.get().findByID(id)) {
    if (this->_client == nullptr) {
        return;
    }
    game::IEntity* entity = ctx.world.get().entityDatabase().query(id);
    if (entity == nullptr) {
        this->_client->sendError();
        return;
    }
    this->_player = dynamic_cast<game::entity::Player*>(entity);
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
