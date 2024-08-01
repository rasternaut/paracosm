include(FetchContent)
set(FETCHCONTENT_QUIET TRUE)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # linux can fetch Vulkan SDK as a archive
    FetchContent_Declare(
        vulkan
        URL https://sdk.lunarg.com/sdk/download/1.3.290.0/linux/vulkansdk-linux-x86_64-1.3.290.0.tar.xz
        URL_HASH SHA256=440906a95e7d42da0eaf5c3ae41471baf4aa7086df9d9b93795b0f908239a76a
    )
    FetchContent_MakeAvailable(vulkan)
    set( ENV{VULKAN_SDK} ${vulkan_SOURCE_DIR}/x86_64)
    set( ENV{PATH} $ENV{VULKAN_SDK}/lib:$ENV{PATH})
    set( ENV{LD_LIBRARY_PATH} $ENV{VULKAN_SDK}/libs:$ENV{LD_LIBRARY_PATH})
    set( ENV{VK_ADD_LAYER_PATH}  $ENV{VULKAN_SDK}/share/vulkan/explicit_layer.d)
endif()


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


if(MSVC_VERSION GREATER_EQUAL 1900)
set(GLFW_BUILD_VERSION "lib-vc2022")
endif()

set(GLFW_LIB_DIR "${glfw_SOURCE_DIR}/${GLFW_BUILD_VERSION}")
set(GLFW_HEADERS_DIR "${glfw_SOURCE_DIR}/include/")

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

find_package(Vulkan REQUIRED)

target_include_directories(imgui
PUBLIC
"${imgui_SOURCE_DIR}/"
)

target_link_libraries( imgui
PRIVATE
Vulkan::Vulkan
glfw
)







# openCV does not play nice with fetch content, need to investiage better build
# FetchContent_Declare(
#     open_cv
#     URL        https://github.com/opencv/opencv/releases/download/4.9.0/opencv-4.9.0-windows.exe
#     URL_HASH   SHA256=fefddff0623fbd5a6fa0cecb9bccd4b822478354e6c587ebb6e40ab09dacba51
# )
