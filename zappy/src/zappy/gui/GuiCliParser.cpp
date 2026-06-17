/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GuiCliParser
*/

#include "zappy/gui/GuiCliParser.hpp"

#include <exception>
#include <limits>
#include <string>
#include <string_view>

#include "zappy/shared/exception/InvalidArgument.hpp"

namespace zappy::gui {

GuiCliParser::GuiCliParser(std::span<const std::string_view> args) {
    for (std::size_t i = 0; i + 1 < args.size(); ++i) {
        if (args[i] == "-h") {
            _host = args[i + 1];
        } else if (args[i] == "-p") {
            try {
                const unsigned long parsed = std::stoul(std::string{args[i + 1]});
                if (parsed > std::numeric_limits<std::uint16_t>::max()) {
                    throw exception::InvalidArgument{"port out of range"};
                }
                _port = static_cast<std::uint16_t>(parsed);
            } catch (const exception::InvalidArgument&) {
                throw;
            } catch (const std::exception&) {
                throw exception::InvalidArgument{"invalid port value"};
            }
        }
    }
    if (_host.empty()) {
        throw exception::InvalidArgument{"missing required argument: -h machine"};
    }
    if (_port == 0) {
        throw exception::InvalidArgument{"missing required argument: -p port"};
    }
}

std::string_view GuiCliParser::host() const { return _host; }
std::uint16_t GuiCliParser::port() const { return _port; }

}  // namespace zappy::gui
