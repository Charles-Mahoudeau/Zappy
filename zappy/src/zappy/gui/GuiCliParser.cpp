/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** GuiCliParser
*/

#include "zappy/gui/GuiCliParser.hpp"

#include <cstdint>
#include <exception>
#include <limits>
#include <span>
#include <string>
#include <string_view>

#include "zappy/shared/exception/InvalidArgument.hpp"

namespace zappy::gui {

GuiCliParser::GuiCliParser(std::span<const std::string_view> args) {
    auto it = args.begin();
    while (it != args.end()) {
        const std::string_view flag = *it;
        ++it;
        if (it == args.end()) {
            break;
        }
        if (flag == "-h") {
            _host = *it;
        } else if (flag == "-p") {
            try {
                const unsigned long parsed = std::stoul(std::string{*it});
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
        ++it;
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
