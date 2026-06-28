#include "KeySystem.hpp"

#include <raylib.h>

#include <utility>

namespace zappy::gui::ui {
bool KeySystem::isKeyPressed(Key key) { return ::IsKeyPressed(std::to_underlying(key)); }
bool KeySystem::IsKeyPressedRepeat(Key key) { return ::IsKeyPressedRepeat(std::to_underlying(key)); }
bool KeySystem::IsKeyDown(Key key) { return ::IsKeyDown(std::to_underlying(key)); }
bool KeySystem::IsKeyReleased(Key key) { return ::IsKeyReleased(std::to_underlying(key)); }
bool KeySystem::IsKeyUp(Key key) { return ::IsKeyUp(std::to_underlying(key)); }
int KeySystem::GetKeyPressed() { return ::GetKeyPressed(); }
}  // namespace zappy::gui::ui
