/*
** EPITECH PROJECT, 2026
** Core
** File description:
** Core code
*/

#include "zappy/server/Core.hpp"

#include <cstdint>
#include <format>
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
#include "zappy/server/game/World.hpp"
#include "zappy/shared/exception/Exception.hpp"
#include "zappy/shared/exception/InvalidState.hpp"
#include "zappy/shared/io/Logger.hpp"
#include "zappy/shared/math/Vector2.hpp"

namespace zappy::server {
void Core::init(const std::span<std::string_view> argv) {
    _logger.info("Initialization begin.");

    CliParser parser{argv};
    const CliParser::CliParameters& parameters = parser.parameters();
    _logger.info("Parameters parsed and validated.");

    // ReSharper disable CppDFAConstantConditions
    if (initTeams(parameters.teamsName) && initNetwork(parameters.port) && initTimer(parameters.frequencies) &&
        initWorld({parameters.mapWidth, parameters.mapHeight}, parameters.teamsName, parameters.nbPlayerPerTeam) &&
        initCommandGroups()) {
        _logger.info("Initialization done.");
    } else {
        _logger.fatal("Error occurred during initialization.");
        throw exception::InvalidState{"initialization error"};
    }
    // ReSharper restore CppDFAConstantConditions
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
            (*commands)(client, req.value());
        }
    }
}

void Core::nextTick() {
    int timeout = this->_timer.timeoutUntilSchedule();

    while (this->_serv.poll(timeout)) {
        timeout = this->_timer.timeoutUntilNextTick();
    }
    this->_timer.update();
    this->_clientRegistry.update();
}

bool Core::initTeams(const std::span<const std::string_view> names) {
    const io::Logger logger = _logger.derive("Teams");

    for (std::string_view teamName : names) {
        try {
            _teamRegistry.createTeam(teamName);
        } catch (const exception::Exception& e) {
            logger.error(std::format("Failed to create team '{}': {}", teamName, e.what()));
            return false;
        }
        logger.info(std::format("Created team '{}'.", teamName));
    }
    _logger.info("Teams initialized.");
    return true;
}

bool Core::initNetwork(const std::uint16_t port) {
    try {
        _serv.init(port, _clientRegistry, _timer);
    } catch (const exception::Exception& e) {
        _logger.error(std::format("Failed to initialize network: {}", e.what()));
        return false;
    }
    _logger.info("Network initialized.");
    return true;
}

// ReSharper disable once CppDFAConstantFunctionResult
bool Core::initTimer(const std::uint16_t frequency) {
    if (frequency == 0) {
        _logger.info("Using default timer frequency.");
        return true;
    }
    _timer.setFrequencies(frequency);
    _logger.info("Timer initialized.");
    return true;
}

bool Core::initWorld(math::Vector2u size, std::span<const std::string_view> teams, std::uint16_t nbPlayerPerTeam) {
    try {
        _world = std::make_unique<game::World>(size, _logger.derive("World"));
        this->_world->spawnStartEggs(teams, nbPlayerPerTeam);
    } catch (const exception::Exception& e) {
        _logger.error(std::format("Failed to initialize world: {}", e.what()));
        return false;
    }
    _logger.info("World initialized.");
    return true;
}

bool Core::initCommandGroups() {
    using enum Client::Type;

    const auto makeGroup = [this]<typename T>() {
        return std::make_unique<T>(command::ICommandGroup::CommandCtx{
            .timer = _timer,
            .clientRegistry = _clientRegistry,
            .teamRegistry = _teamRegistry,
            .world = *_world,
            .logger = _logger,
        });
    };

    _cmdGroups.emplace(kPlayer, makeGroup.operator()<command::PlayerCommands>());
    _cmdGroups.emplace(kGui, makeGroup.operator()<command::GuiCommands>());
    _cmdGroups.emplace(kUnknown, makeGroup.operator()<command::UnknownCommands>());
    _logger.info("Command groups initialized.");
    return true;
}
}  // namespace zappy::server
