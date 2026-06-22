include(FetchContent)
include(InternetCheck)

check_internet_connection()

macro(setup_beman_anyview)
    if (INTERNET_AVAILABLE)
        FetchContent_Declare(
                beman.any_view
                GIT_REPOSITORY https://github.com/bemanproject/any_view.git
                GIT_TAG v1.1.0
        )

        FetchContent_MakeAvailable(beman.any_view)
    else ()
        message(FATAL_ERROR
                "beman.any_view is required but could not be fetched (no internet connection). "
                "Provide a vendored/local copy or enable network access during configure.")
    endif ()
endmacro()
