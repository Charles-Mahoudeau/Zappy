include(SetupRaylib)

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
        render/tools/Vec2D.cpp
        render/tools/Vec2D.hpp
        render/objects/Texture.cpp
        render/objects/Texture.hpp
        render/objects/Billboard.cpp
        render/objects/Billboard.hpp
        render/objects/Model.cpp
        render/objects/Model.hpp
        render/objects/Skybox.cpp
        render/objects/Skybox.hpp
        render/utils/Color.hpp
        render/utils/Vector3.hpp
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
