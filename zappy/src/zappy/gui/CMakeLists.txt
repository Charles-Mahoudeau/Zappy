include(FetchContent)
include(InternetCheck)

check_internet_connection()

if (INTERNET_AVAILABLE)
    FetchContent_Declare(
            raylib
            GIT_REPOSITORY https://github.com/raysan5/raylib.git
            GIT_TAG 5.0
    )
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
    FetchContent_MakeAvailable(raylib)
else ()
    message(WARNING "Raylib not available: no internet connection.")
endif ()

add_library(zappy_gui_objects OBJECT
        Demo.cpp
        Demo.hpp
        display/IWindow.hpp
        display/Window.cpp
        display/Window.hpp
        network/LineBuffer.cpp
        network/LineBuffer.hpp
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
