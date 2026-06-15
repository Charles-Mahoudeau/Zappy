/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** ProtocolParser
*/

#pragma once

#include <functional>
#include <string_view>

#include "zappy/gui/game/GameState.hpp"
#include "zappy/gui/network/HandlerFactory.hpp"

namespace zappy::gui::network {

class ProtocolParser {
  public:
    explicit ProtocolParser(game::GameState& state);
    ~ProtocolParser() = default;

    ProtocolParser(const ProtocolParser&) = delete;
    ProtocolParser& operator=(const ProtocolParser&) = delete;

    ProtocolParser(ProtocolParser&&) noexcept = default;
    ProtocolParser& operator=(ProtocolParser&&) noexcept = default;

    void dispatch(std::string_view line);

  private:
    std::reference_wrapper<game::GameState> _state;
    HandlerMap _handlers;
};

}  // namespace zappy::gui::network
