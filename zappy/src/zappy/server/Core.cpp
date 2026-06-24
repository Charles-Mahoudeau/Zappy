/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core code
*/

#include "zappy/server/Core.hpp"

#include <exception>
#include <format>
#include <iostream>
#include <span>
#include <string_view>

#include "zappy/server/CliParser.hpp"
#include "zappy/server/client/Client.hpp"
#include "zappy/shared/exception/Exception.hpp"

namespace zappy::server {
void Core::init(const std::span<std::string_view> argv) {
    _logger.info("Begin initialization");

    CliParser parser{argv};
    const CliParser::CliParameters& parameters = parser.parameters();

    _logger.info("Initializing teams");
    try {
        for (std::string_view teamName : parameters.teamsName) {
            _logger.info(std::format("Creating team '{}'", teamName));
            _teamRegistry.createTeam(std::string{teamName});
        }
    } catch (const std::exception& e) {
        _logger.fatal(std::format("Failed to initialize teams: {}", e.what()));
        throw;
    }

    _logger.info("Initializing network layer");
    this->_serv.init(parameters.port, this->_clientRegistry, this->_time);

    _logger.info("Initializing timer");
    this->_time.setFrequencies(parameters.frequencies);

    _logger.info("Initialization done!");
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

void Core::nextTick() {
    int timeout = this->_time.timeoutUntilSchedule();

    while (this->_serv.poll(timeout)) {
        timeout = this->_time.timeoutUntilNextTick();
    }
    this->_time.update();
    this->_clientRegistry.update();
}
}  // namespace zappy::server
