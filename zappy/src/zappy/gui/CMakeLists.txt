include(SetupRaylib)
setup_raylib()

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
        render/Camera.cpp
        render/Camera.hpp
        render/objects/Texture.cpp
        render/objects/Texture.hpp
        render/objects/Model.cpp
        render/objects/Model.hpp
        render/objects/Skybox.cpp
        render/objects/Skybox.hpp
        render/utils/Color.hpp
        render/utils/Vector3.hpp
        ui/ChatPanel.cpp
        ui/ChatPanel.hpp
        ui/GuiTheme.cpp
        ui/GuiTheme.hpp
        ui/InfoPanel.cpp
        ui/InfoPanel.hpp
        ui/Mouse.cpp
        ui/Mouse.hpp
        ui/RaylibGuiImpl.cpp
        ui/TimeUnitSlider.cpp
        ui/TimeUnitSlider.hpp
        ui/utils/Rectangle.hpp
        ui/utils/Vector2.hpp
        ui/Widgets.cpp
        ui/Widgets.hpp
)
target_include_directories(zappy_gui_objects PUBLIC
        ${CMAKE_SOURCE_DIR}/src
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
