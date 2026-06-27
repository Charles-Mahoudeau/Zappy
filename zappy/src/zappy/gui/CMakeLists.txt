include(SetupRaylib)

include(SetupRaygui)
setup_raygui()

add_library(zappy_gui_objects OBJECT
        display/Window.cpp
        display/Window.hpp
        game/GameState.cpp
        game/GameState.hpp
        GUI.cpp
        GUI.hpp
        GuiCliParser.cpp
        GuiCliParser.hpp
        network/CommandSender.cpp
        network/CommandSender.hpp
        network/Handshake.cpp
        network/Handshake.hpp
        network/HandlerRegistry.cpp
        network/HandlerRegistry.hpp
        network/ProtocolParser.cpp
        network/ProtocolParser.hpp
        render/AssetLoaderRegistry.cpp
        render/AssetLoaderRegistry.hpp
        render/AssetStore.cpp
        render/AssetStore.hpp
        render/Renderer.cpp
        render/Renderer.hpp
        render/Grid.cpp
        render/Grid.hpp
        render/Camera.cpp
        render/Camera.hpp
        render/objects/Texture.cpp
        render/objects/Texture.hpp
        render/objects/Billboard.cpp
        render/objects/Billboard.hpp
        render/objects/Model.cpp
        render/objects/Model.hpp
        render/objects/Skybox.cpp
        render/objects/Skybox.hpp
        render/objects/particles/ParticleEmitter.cpp
        render/objects/particles/ParticleEmitter.hpp
        render/objects/particles/Particle.cpp
        render/objects/particles/Particle.hpp
        render/utils/Vec2D.cpp
        render/utils/Vec2D.hpp
        render/utils/Color.hpp
        render/utils/ColorF.hpp
        render/utils/ColorF.cpp
        render/utils/Ray.hpp
        render/utils/Rectangle.hpp
        render/utils/Vector2.hpp
        render/utils/Vector3.hpp
        render/utils/Randomvalue.hpp
        render/utils/TimeValue.hpp
        ui/ChatPanel.cpp
        ui/ChatPanel.hpp
        ui/Draggable.cpp
        ui/Draggable.hpp
        ui/GuiTheme.cpp
        ui/GuiTheme.hpp
        ui/Hud.cpp
        ui/Hud.hpp
        ui/InfoPanel.cpp
        ui/InfoPanel.hpp
        ui/Leaderboard.cpp
        ui/Leaderboard.hpp
        ui/Mouse.cpp
        ui/Mouse.hpp
        ui/PlayerInfo.cpp
        ui/PlayerInfo.hpp
        ui/RaylibGuiImpl.cpp
        ui/TileInfo.cpp
        ui/TileInfo.hpp
        ui/TimeUnitSlider.cpp
        ui/TimeUnitSlider.hpp
        ui/VictoryScreen.cpp
        ui/VictoryScreen.hpp
        ui/Widgets.cpp
        ui/Widgets.hpp
)
target_include_directories(zappy_gui_objects PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/zappy/gui/render/objects
    ${CMAKE_SOURCE_DIR}/src/zappy/gui/render/objects/particles
    ${CMAKE_SOURCE_DIR}/src/zappy/gui/render/utils
    ${CMAKE_SOURCE_DIR}/src/zappy/gui/render
)

target_link_libraries(zappy_gui_objects PUBLIC
        zappy_shared
)

if (TARGET raylib)
    target_link_libraries(zappy_gui_objects PUBLIC raylib)
endif ()

if (DEFINED raygui_SOURCE_DIR)
    target_include_directories(zappy_gui_objects PUBLIC ${raygui_SOURCE_DIR}/src)
endif ()

add_executable(zappy_gui
        main.cpp
)
target_link_libraries(zappy_gui PRIVATE
        zappy_shared
        zappy_gui_objects
)

# Copy GUI assets next to the executable so they ship with the binary
add_custom_command(TARGET zappy_gui POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:zappy_gui>/assets
)
