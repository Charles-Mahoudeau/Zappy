add_library(zappy_server_objects OBJECT
        Demo.cpp
        Demo.hpp
)
target_include_directories(zappy_server_objects PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(zappy_server_objects PUBLIC
        zappy_shared
)

add_executable(zappy_server
        main.cpp
        game/EntityDatabase.cpp
        game/EntityDatabase.hpp
        game/IEntity.hpp
)
target_link_libraries(zappy_server PRIVATE
        zappy_shared
        zappy_server_objects
)
