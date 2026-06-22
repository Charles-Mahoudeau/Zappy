add_library(zappy_server_objects OBJECT
        Core.hpp
        Core.cpp
        CliParser.hpp
        CliParser.cpp
        game/EntityDatabase.cpp
        game/EntityDatabase.hpp
        game/IEntity.hpp
        game/ResourceType.hpp
        game/Inventory.cpp
        game/Inventory.hpp
        game/entity/Player.cpp
        game/entity/Player.hpp
        game/entity/Egg.cpp
        game/entity/Egg.hpp
        game/entity/Resource.cpp
        game/entity/Resource.hpp
        net/SocketRegistry.hpp
        net/SocketRegistry.cpp
        client/Client.hpp
        client/Client.cpp
        client/ClientRegistry.hpp
        client/ClientRegistry.cpp
        game/World.cpp
        game/World.hpp
        game/Tile.cpp
        game/Tile.hpp
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
