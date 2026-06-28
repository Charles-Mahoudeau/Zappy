# Zappy – GUI

The Zappy GUI is a real-time 3D spectator client: it connects to the server in `GRAPHIC` mode, reconstructs the
game state from the protocol, and renders it live. It does not play the game, it only observes it.

## Why raylib?

A simple, function-based 3D/windowing API (no editor, no engine workflow) that fits a project-sized binary
rather than a full game engine. It comes with GPU skinning for the animated slug models, ships as a single
CMake `FetchContent` dependency, and its plain C structs are easy to wrap in small typed C++ classes
(`zappy::gui::render::Vector3`, `Color`, `Rectangle`, ...).

## Why raygui?

raylib's own immediate-mode UI library, drawn straight into the same 3D frame with no separate UI thread or
widget tree to keep in sync. Immediate-mode also matches how the GUI already redraws everything from
`GameState` every frame. All calls go through `zappy::gui::ui::Widgets`, which also applies the project's
custom `.rgs` style.

## Goals of the GUI

* **Mirror the server faithfully**: everything shown (map, tiles, players, eggs, broadcasts, time unit,
  winner) comes straight from the GRAPHIC protocol stream, never assumed.
* **Stay usable without the server**: shows a "Waiting for server..." screen and reconnects automatically
  instead of exiting if the server isn't up or drops.
* **Make the simulation legible**: animated, interpolated player movement, camera focus-follow, and VFX
  (incantation, death, resource) so the game state is easy to *see*, not just to read in a panel.
* **Lean into the Slugterra theme**: players are Slugterra slugs (Burpy, Doc, Electra, Frozy, ...), resources
  are glowing energy crystals, and the map is staged as floating islands and bridges under a cavern skybox.
* **Keep the UI and the 3D view from fighting each other**: panels are draggable/scrollable/clickable, but
  `zappy::gui::ui::Hud` arbitrates so UI input never leaks into the camera's own mouse/scroll handling.
