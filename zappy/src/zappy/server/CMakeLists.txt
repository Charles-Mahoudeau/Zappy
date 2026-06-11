add_library(zappy_server_objects OBJECT
        Core.hpp
        Core.cpp
        CliParser.hpp
        CliParser.cpp
        game/EntityDatabase.cpp
        game/EntityDatabase.hpp
        game/IEntity.hpp
        game/ResourceType.hpp
)

target_include_directories(zappy_server_objects PUBLIC
        ${CMAKE_SOURCE_DIR}/src
)
target_link_libraries(zappy_server_objects PUBLIC
        zappy_shared
)

add_executable(zappy_server
        main.cpp
)
target_link_libraries(zappy_server PRIVATE
        zappy_shared
        zappy_server_objects
)
