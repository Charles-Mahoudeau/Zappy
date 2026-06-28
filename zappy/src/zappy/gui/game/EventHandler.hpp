/*
** EPITECH PROJECT, 2026
** zappy
** File description:
** EventHandler
*/

#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <string_view>
#include <vector>

#include "zappy/gui/render/utils/Vector3.hpp"

namespace zappy::gui::game {

enum class EventType : std::uint8_t { None, Broadcast, Incantation, Death, EggHatch, LevelUp, Eat, Push, EggDeath };

struct Event {
    EventType type{EventType::None};
    render::Vector3 position{0.0F, 0.0F, 0.0F};
};

struct EventResponse {
    std::reference_wrapper<std::vector<std::string_view>> emitters;
    render::Vector3 position{0.0F, 0.0F, 0.0F};
};

class EventHandler {
  public:
    EventHandler() = default;
    EventHandler(const EventHandler&) = default;
    EventHandler& operator=(const EventHandler&) = default;
    EventHandler(EventHandler&&) = default;
    EventHandler& operator=(EventHandler&&) = default;
    ~EventHandler() = default;

    EventResponse handleEvent(Event& event) {
        const auto position = event.position;
        const auto it = _eventHandlers.find(event.type);
        event = Event{};
        if (it != _eventHandlers.end()) {
            return EventResponse{.emitters = std::ref(it->second), .position = position};
        }
        static std::vector<std::string_view> emptyVector;
        return EventResponse{.emitters = std::ref(emptyVector), .position = position};
    }

  private:
    std::map<EventType, std::vector<std::string_view>> _eventHandlers{
        {EventType::Broadcast, {"singal"}},     {EventType::Incantation, {"sparkles"}},
        {EventType::Death, {"smoke", "skull"}}, {EventType::EggHatch, {"eggCrack"}},
        {EventType::LevelUp, {"slash"}},        {EventType::Eat, {"foodDebris"}},
        {EventType::Push, {"impact"}},          {EventType::EggDeath, {"smoke"}}};
};

}  // namespace zappy::gui::game
