add_library(zappy_server_objects OBJECT
	Core.hpp
	Core.cpp
	world/Map.hpp
	world/Map.cpp
	CliParsing.hpp
	CliParsing.cpp
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
