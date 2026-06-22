/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** ChatPanel
*/

#include "ChatPanel.hpp"

#include <algorithm>

#include "Widgets.hpp"

namespace zappy::gui::ui {

Rectangle ChatPanel::contentRect(Rectangle bounds, std::size_t messageCount) {
    return Rectangle{0.0F, 0.0F, bounds.width(), static_cast<float>(messageCount) * kLineHeight};
}

bool ChatPanel::consumeAutoScrollReset(std::size_t messageCount) {
    if (messageCount == _lastMessageCount) {
        return false;
    }
    _lastMessageCount = messageCount;
    return true;
}

void ChatPanel::draw(const std::deque<std::string>& broadcasts, Rectangle bounds) {
    if (consumeAutoScrollReset(broadcasts.size())) {
        const float fullHeight = static_cast<float>(broadcasts.size()) * kLineHeight;
        _scroll = Vector2{0.0F, -std::max(0.0F, fullHeight - bounds.height())};
    }

    const Rectangle content = contentRect(bounds, broadcasts.size());
    const Rectangle view = Widgets::scrollPanel(bounds, "Chat", content, _scroll);

    float y = bounds.y() + _scroll.y();
    for (const auto& message : broadcasts) {
        Widgets::label(Rectangle{bounds.x(), y, content.width(), kLineHeight}, message);
        y += kLineHeight;
    }
    static_cast<void>(view);
}

}  // namespace zappy::gui::ui
