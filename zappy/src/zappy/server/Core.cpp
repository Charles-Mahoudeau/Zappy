/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core code
*/

#include "zappy/server/Core.hpp"

#include <unistd.h>

#include <iostream>
#include <span>
#include <string_view>

#include "zappy/server/CliParser.hpp"
#include "zappy/server/client/Client.hpp"

namespace zappy::server {

void Core::init(std::span<std::string_view> argv) {
    CliParser parser{argv};
    const CliParser::CliParameters& parameters = parser.parameters();

    this->_serv.init(parameters.port, this->_clientRegistry, this->_time);
    this->_time.init(parameters.frequencies);
}

void Core::run() {
    this->_time.scheduleEvery(10, []() { std::cout << "eat\n"; });
    while (true) {
        this->nextTick();
        this->processCommands();
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
        std::cout << req.value() << "\n";

        // command logic
    }
}
}  // namespace zappy::server
