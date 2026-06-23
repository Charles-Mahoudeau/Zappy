/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core code
*/

#include "zappy/server/Core.hpp"

#include <iostream>
#include <span>
#include <string_view>

#include "zappy/server/CliParser.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/shared/exception/Exception.hpp"

namespace zappy::server {

void Core::init(std::span<std::string_view> argv) {
    CliParser parser{argv};
    const CliParser::CliParameters& parameters = parser.parameters();

    this->_serv.init(parameters.port, this->_clientRegistry, this->_time);
    this->_time.setFrequencies(parameters.frequencies);
}

void Core::run() {
    while (true) {
        try {
            this->nextTick();
            this->processCommands();
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

void Core::processCommands() {
    for (Client* client : this->_clientRegistry.viewAll()) {
        if (client->inTimeout()) {
            continue;
        }

        auto req = client->nextRequest();
        if (!req.has_value()) {
            continue;
        }

        // command logic
    }
}

}  // namespace zappy::server
