include(FetchContent)

FetchContent_Declare(
        beman.any_view
        GIT_REPOSITORY https://github.com/bemanproject/any_view.git
        GIT_TAG v1.1.0
        FIND_PACKAGE_ARGS NAMES beman.any_view CONFIG
)

FetchContent_MakeAvailable(beman.any_view)
