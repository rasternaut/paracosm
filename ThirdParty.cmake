# Setup Raylib
# sourced from https://github.com/raysan5/raylib/blob/master/projects/CMake/CMakeLists.txt

set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )

    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_MakeAvailable(raylib)
    endif()
endif()

set(RAYGUI_VERSION 4.0)

include(FetchContent)
FetchContent_Declare(
raygui_external
DOWNLOAD_EXTRACT_TIMESTAMP OFF
URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYGUI_VERSION}.tar.gz
)
FetchContent_MakeAvailable(raygui_external)
FetchContent_GetProperties(raygui_external)

add_library(raygui INTERFACE)
target_include_directories(raygui INTERFACE ${raygui_external_SOURCE_DIR}/src/)

