/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** CommandSender
*/

#include "zappy/gui/network/CommandSender.hpp"

#include <cstdint>
#include <format>

#include "zappy/shared/exception/InvalidArgument.hpp"
#include "zappy/shared/network/BufferedClient.hpp"

namespace zappy::gui::network {

CommandSender::CommandSender(zappy::network::BufferedClient& buffer) : _buffer{buffer} {}

void CommandSender::requestPlayer(std::uint32_t id) const {
    _buffer.get().send(std::format("ppo #{}\n", id));
    _buffer.get().send(std::format("plv #{}\n", id));
    _buffer.get().send(std::format("pin #{}\n", id));
}

void CommandSender::requestTile(std::uint32_t x, std::uint32_t y) const {
    _buffer.get().send(std::format("bct {} {}\n", x, y));
}

void CommandSender::setTimeUnit(int t) const {
    if (t <= 0) {
        throw exception::InvalidArgument{"time unit must be strictly positive"};
    }
    _buffer.get().send(std::format("sst {}\n", t));
}

void CommandSender::requestInitialState() const {
    _buffer.get().send("msz\n");
    _buffer.get().send("mct\n");
    _buffer.get().send("tna\n");
    _buffer.get().send("sgt\n");
}

}  // namespace zappy::gui::network
