include(FetchContent)
set(FETCHCONTENT_QUIET TRUE)

# fetch and set up 3rd party utils

# FetchContent_Declare(
#     glfw
#     URL         https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip
# )

FetchContent_Declare (
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579
    GIT_PROGRESS TRUE
)

FetchContent_Declare(
    imgui
    URL         https://github.com/ocornut/imgui/archive/refs/tags/v1.90.4-docking.zip
)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")


set(FETCHCONTENT_QUIET FALSE)
FetchContent_MakeAvailable(imgui glfw)

# find_package(OpenCV REQUIRED PATHS ${open_cv_SOURCE_DIR}/build/)
# set(opencv_dll_location  ${open_cv_SOURCE_DIR}/build/x64/vc16/bin/)


# TODO: rest of MSVC versions as supported by GLFW

if(MSVC_VERSION GREATER_EQUAL 1900)
set(GLFW_BUILD_VERSION "lib-vc2022")
endif()

set(GLFW_LIB_DIR "${glfw_SOURCE_DIR}/${GLFW_BUILD_VERSION}")
set(GLFW_HEADERS_DIR "${glfw_SOURCE_DIR}/include/")

# add_library(glfw STATIC IMPORTED)
# set_target_properties(glfw PROPERTIES
# IMPORTED_LOCATION "${GLFW_LIB_DIR}/glfw3_mt.lib"
# IMPORTED_LOCATION_DEBUG  "${GLFW_LIB_DIR}/glfw3_mt.lib"
# INTERFACE_INCLUDE_DIRECTORIES "${GLFW_HEADERS_DIR}"
# )


add_library(imgui STATIC

"${imgui_SOURCE_DIR}/imconfig.h"
"${imgui_SOURCE_DIR}/imgui.h"
"${imgui_SOURCE_DIR}/imgui_internal.h"
"${imgui_SOURCE_DIR}/imstb_rectpack.h"
"${imgui_SOURCE_DIR}/imstb_textedit.h"
"${imgui_SOURCE_DIR}/imstb_truetype.h"

"${imgui_SOURCE_DIR}/imgui.cpp"
"${imgui_SOURCE_DIR}/imgui_demo.cpp"
"${imgui_SOURCE_DIR}/imgui_draw.cpp"
"${imgui_SOURCE_DIR}/imgui_tables.cpp"
"${imgui_SOURCE_DIR}/imgui_widgets.cpp"

"${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.h"
"${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp"
"${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h"
"${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
)

target_include_directories(imgui
PUBLIC
"${imgui_SOURCE_DIR}/"
)

target_link_libraries( imgui
PRIVATE
Vulkan::Vulkan
glfw
)



find_package(Vulkan REQUIRED)




# openCV does not play nice with fetch content, need to investiage better build
# FetchContent_Declare(
#     open_cv
#     URL        https://github.com/opencv/opencv/releases/download/4.9.0/opencv-4.9.0-windows.exe
#     URL_HASH   SHA256=fefddff0623fbd5a6fa0cecb9bccd4b822478354e6c587ebb6e40ab09dacba51
# )
