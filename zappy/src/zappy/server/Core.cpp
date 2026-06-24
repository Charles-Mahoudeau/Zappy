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

    if (parameters.frequencies != 0) {
        this->_time.setFrequencies(parameters.frequencies);
    }

    using enum Client::Type;

    const auto makeGroup = [this]<typename T> {
        return std::make_unique<T>(this->_time, this->_clientRegistry, *this->_world, this->_logger);
    };

    this->_cmdGroups.emplace(kPlayer, makeGroup.operator()<command::PlayerCommands>());
    this->_cmdGroups.emplace(kGui, makeGroup.operator()<command::GuiCommands>());
    this->_cmdGroups.emplace(kUnknown, makeGroup.operator()<command::UnknownCommands>());
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
