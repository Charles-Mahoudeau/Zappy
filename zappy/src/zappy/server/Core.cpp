/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core code
*/

#include "zappy/server/Core.hpp"

#include <iostream>
#include <memory>
#include <span>
#include <string_view>

#include "zappy/server/CliParser.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/server/commands/GuiCommands.hpp"
#include "zappy/server/commands/ICommandGroup.hpp"
#include "zappy/server/commands/PlayerCommands.hpp"
#include "zappy/server/commands/UnknownCommands.hpp"
#include "zappy/shared/exception/Exception.hpp"

namespace zappy::server {

void Core::init(std::span<std::string_view> argv) {
    CliParser parser{argv};
    const CliParser::CliParameters& parameters = parser.parameters();

    this->_serv.init(parameters.port, this->_clientRegistry, this->_time);
    this->_time.setFrequencies(parameters.frequencies);

    this->_cmdGroups.emplace(Client::Type::kPlayer,
                             std::make_unique<command::PlayerCommands>(this->_time, this->_clientRegistry));
    this->_cmdGroups.emplace(Client::Type::kGui,
                             std::make_unique<command::GuiCommands>(this->_time, this->_clientRegistry));
    this->_cmdGroups.emplace(Client::Type::kUnknown,
                             std::make_unique<command::UnknownCommands>(this->_time, this->_clientRegistry));
}

void Core::run() {
    while (true) {
        try {
            this->nextTick();
            this->processCommandGroup();
        } catch (const exception::Exception& err) {
            std::cerr << "Error: " << err.what() << "\n";
        }
    }
}

void Core::nextTick() {
    int timeout = this->_time.timeoutUntilSchedule();

    while (this->_serv.poll(timeout)) {
        timeout = this->_time.timeoutUntilNextTick();
    }
    this->_time.update();
    this->_clientRegistry.update();
}

void Core::processCommandGroup() {
    for (Client* client : this->_clientRegistry.viewAll()) {
        if (client->inTimeout()) {
            continue;
        }

        auto req = client->nextRequest();
        if (!req.has_value()) {
            continue;
        }

        if (auto iter = this->_cmdGroups.find(client->type()); iter != this->_cmdGroups.end()) {
            const auto& commands = iter->second;
            commands->execute(client, req.value());
        }
    }
}

}  // namespace zappy::server
