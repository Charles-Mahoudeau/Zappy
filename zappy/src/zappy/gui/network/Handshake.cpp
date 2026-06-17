/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Handshake
*/

#include "zappy/gui/network/Handshake.hpp"

#include <chrono>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/CommandSender.hpp"
#include "zappy/gui/network/ProtocolParser.hpp"
#include "zappy/shared/exception/InvalidState.hpp"
#include "zappy/shared/network/BufferedClient.hpp"

namespace zappy::gui::network {

static constexpr auto kTimeout = std::chrono::seconds{5};

Handshake::Handshake(zappy::network::BufferedClient& buffer, CommandSender& sender, ProtocolParser& parser,
                     game::GameState& state)
    : _buffer{buffer}, _sender{sender}, _parser{parser}, _state{state} {}

void Handshake::run() {
    const auto deadline = std::chrono::steady_clock::now() + kTimeout;

    while (!_buffer.get().hasMessages()) {
        if (std::chrono::steady_clock::now() > deadline) {
            throw exception::InvalidState{"handshake timeout: no WELCOME received"};
        }
        _buffer.get().poll();
    }

    if (_buffer.get().popMessage() != "WELCOME") {
        throw exception::InvalidState{"handshake failed: expected WELCOME"};
    }

    _buffer.get().send("GRAPHIC\n");
    _sender.get().requestInitialState();

    const auto stateDeadline = std::chrono::steady_clock::now() + kTimeout;
    while (!_state.get().isReady()) {
        while (_buffer.get().hasMessages()) {
            _parser.get().dispatch(_buffer.get().popMessage());
        }
        if (std::chrono::steady_clock::now() > stateDeadline) {
            throw exception::InvalidState{"handshake timeout: initial state not received"};
        }
        _buffer.get().poll();
    }
}

}  // namespace zappy::gui::network
