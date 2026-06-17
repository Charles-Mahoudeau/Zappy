include(SetupRaylib)
setup_raylib()

add_library(zappy_gui_objects OBJECT
        Demo.cpp
        Demo.hpp
        display/Window.cpp
        display/Window.hpp
        game/GameState.cpp
        game/GameState.hpp
        network/CommandSender.cpp
        network/CommandSender.hpp
        network/Handshake.cpp
        network/Handshake.hpp
        network/HandlerRegistry.cpp
        network/HandlerRegistry.hpp
        network/ProtocolParser.cpp
        network/ProtocolParser.hpp
        render/Camera.cpp
        render/Camera.hpp
        render/objects/Texture.cpp
        render/objects/Texture.hpp
        render/objects/Model.cpp
        render/objects/Model.hpp
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
