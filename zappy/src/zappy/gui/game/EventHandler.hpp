/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EventHandler
*/

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace zappy::gui::game {

enum class Event : std::uint8_t { None, Broadcast, Incantation, Death, EggHatch, LevelUp, Eat, Push };

class EventHandler {
  public:
    EventHandler() = default;
    ~EventHandler() = default;

    std::vector<std::string>& handleEvent(Event& event) {
        auto it = _eventHandlers.find(event);

        event = Event::None;
        if (it != _eventHandlers.end()) {
            return it->second;
        }
        static std::vector<std::string> emptyVector;
        return emptyVector;
    }

  private:
    std::map<Event, std::vector<std::string>> _eventHandlers;
};

}  // namespace zappy::gui::game
