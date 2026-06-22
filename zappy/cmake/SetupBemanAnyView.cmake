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
        message(WARNING "beman.any_view not available: no internet connection.")
    endif ()
endmacro()
