/*
** EPITECH PROJECT, 2026
** ACommandGroup
** File description:
** ACommandGroup code
*/

#include "zappy/server/commands/ACommandGroup.hpp"

#include <string_view>

#include "zappy/server/Timer.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/client/ClientRegistry.hpp"
namespace zappy::server::command {

ACommandGroup::ACommandGroup(Timer& timer, client::ClientRegistry& clients) : _timer(timer), _clients(clients) {}

Timer& ACommandGroup::timer() { return this->_timer; }

client::ClientRegistry& ACommandGroup::clients() { return this->_clients; }

void ACommandGroup::operator()(Client* client, std::string_view cmd) { this->execute(client, cmd); }

}  // namespace zappy::server::command
