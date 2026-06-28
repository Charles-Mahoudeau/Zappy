/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EventHandler
*/

#pragma once

#include <cstdint>
#include <map>
#include <string_view>
#include <vector>

namespace zappy::gui::game {

enum class Event : std::uint8_t { None, Broadcast, Incantation, Death, EggHatch, LevelUp, Eat, Push };

class EventHandler {
  public:
    EventHandler() = default;
    ~EventHandler() = default;

    std::vector<std::string_view>& handleEvent(Event& event) {
        auto it = _eventHandlers.find(event);

        event = Event::None;
        if (it != _eventHandlers.end()) {
            return it->second;
        }
        static std::vector<std::string_view> emptyVector;
        return emptyVector;
    }

  private:
    std::map<Event, std::vector<std::string_view>> _eventHandlers{
        {Event::Broadcast, {"singal"}},  {Event::Incantation, {"sparkles"}}, {Event::Death, {"smoke", "skull"}},
        {Event::EggHatch, {"eggCrack"}}, {Event::LevelUp, {"slash"}},        {Event::Eat, {"foodDebris"}},
        {Event::Push, {"impact"}}};
};

}  // namespace zappy::gui::game
