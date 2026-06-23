include(FetchContent)

FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 6.0
        FIND_PACKAGE_ARGS NAMES raylib CONFIG
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
set(SUPPORT_GPU_SKINNING ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(raylib)
