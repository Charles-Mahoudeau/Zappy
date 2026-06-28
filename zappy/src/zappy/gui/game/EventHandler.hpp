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

#include "Vector3.hpp"

namespace zappy::gui::game {

enum class EventType : std::uint8_t { None, Broadcast, Incantation, Death, EggHatch, LevelUp, Eat, Push };

struct Event {
    EventType type{EventType::None};
    Vector3 position{0.0F, 0.0F, 0.0F};
};

struct EventResponse {
    std::vector<std::string_view>& emitters;
    Vector3 position{0.0F, 0.0F, 0.0F};
};

class EventHandler {
  public:
    EventHandler() = default;
    ~EventHandler() = default;

    EventResponse handleEvent(Event& event) {
        auto it = _eventHandlers.find(event.type);

        event = Event{};
        if (it != _eventHandlers.end()) {
            return {it->second, event.position};
        }
        static std::vector<std::string_view> emptyVector;
        return {emptyVector, event.position};
    }

  private:
    std::map<EventType, std::vector<std::string_view>> _eventHandlers{
        {EventType::Broadcast, {"singal"}},     {EventType::Incantation, {"sparkles"}},
        {EventType::Death, {"smoke", "skull"}}, {EventType::EggHatch, {"eggCrack"}},
        {EventType::LevelUp, {"slash"}},        {EventType::Eat, {"foodDebris"}},
        {EventType::Push, {"impact"}}};
};

}  // namespace zappy::gui::game
