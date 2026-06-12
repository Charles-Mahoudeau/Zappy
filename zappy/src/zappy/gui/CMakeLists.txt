include(SetupRaylib)
setup_raylib()

add_library(zappy_gui_objects OBJECT
        Demo.cpp
        Demo.hpp
        display/Window.cpp
        display/Window.hpp
        game/GameState.cpp
        game/GameState.hpp
        network/HandlerFactory.cpp
        network/HandlerFactory.hpp
        render/Camera.cpp
        render/Camera.hpp
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
