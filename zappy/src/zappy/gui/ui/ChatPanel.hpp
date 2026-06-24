/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ChatPanel
*/

#pragma once

#include <cstddef>
#include <deque>
#include <string>

#include "Draggable.hpp"
#include "utils/Rectangle.hpp"
#include "utils/Vector2.hpp"

namespace zappy::gui::ui {

class ChatPanel {
  public:
    ChatPanel() = default;
    ~ChatPanel() = default;

    ChatPanel(const ChatPanel&) = delete;
    ChatPanel& operator=(const ChatPanel&) = delete;

    ChatPanel(ChatPanel&&) noexcept = default;
    ChatPanel& operator=(ChatPanel&&) noexcept = default;

    void draw(const std::deque<std::string>& broadcasts, Rectangle bounds);

    [[nodiscard]] static Rectangle contentRect(Rectangle bounds, std::size_t messageCount);

    bool consumeAutoScrollReset(std::size_t messageCount);

  private:
    static constexpr float kLineHeight = 20.0F;

    std::size_t _lastMessageCount{0};
    Vector2 _scroll;
    Draggable _drag;
};

}  // namespace zappy::gui::ui
