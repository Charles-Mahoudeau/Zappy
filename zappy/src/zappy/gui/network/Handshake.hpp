/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** Handshake
*/

#pragma once

#include <functional>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/CommandSender.hpp"
#include "zappy/gui/network/ProtocolParser.hpp"
#include "zappy/shared/network/BufferedClient.hpp"

namespace zappy::gui::network {

class Handshake {
  public:
    Handshake(zappy::network::BufferedClient& buffer, CommandSender& sender,
              ProtocolParser& parser, game::GameState& state);
    ~Handshake() = default;

    Handshake(const Handshake&) = delete;
    Handshake& operator=(const Handshake&) = delete;

    Handshake(Handshake&&) noexcept = default;
    Handshake& operator=(Handshake&&) noexcept = default;

    void run();

  private:
    std::reference_wrapper<zappy::network::BufferedClient> _buffer;
    std::reference_wrapper<CommandSender> _sender;
    std::reference_wrapper<ProtocolParser> _parser;
    std::reference_wrapper<game::GameState> _state;
};

}  // namespace zappy::gui::network
