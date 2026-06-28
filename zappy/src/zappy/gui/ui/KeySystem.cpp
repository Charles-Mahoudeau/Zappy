#include "KeySystem.hpp"

#include <raylib.h>

namespace zappy::gui::ui {
bool KeySystem::isKeyPressed(Key key) { return ::IsKeyPressed(static_cast<int>(key)); }
bool KeySystem::IsKeyPressedRepeat(Key key) { return ::IsKeyPressedRepeat(static_cast<int>(key)); }
bool KeySystem::IsKeyDown(Key key) { return ::IsKeyDown(static_cast<int>(key)); }
bool KeySystem::IsKeyReleased(Key key) { return ::IsKeyReleased(static_cast<int>(key)); }
bool KeySystem::IsKeyUp(Key key) { return ::IsKeyUp(static_cast<int>(key)); }
int KeySystem::GetKeyPressed() { return ::GetKeyPressed(); }
}  // namespace zappy::gui::ui
