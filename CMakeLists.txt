
# Mandatory CMake Boilerplate
cmake_minimum_required(VERSION 3.24)

project(
    map-reader
    VERSION 0.0
    LANGUAGES CXX
)

# Specify your C++ version here
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ThirdParty.cmake)


# add_executable(

#     main.cpp
#     ${imgui_source_files}
# )

# target_link_libraries( map_reader
#     Vulkan::Vulkan
#     glfw
#     imgui
#     ${OpenCV_LIBS}
#  )

add_subdirectory(src)


# file(
#     COPY
#         ${opencv_dll_location}/opencv_world490d.dll
#     DESTINATION
#         ${CMAKE_CURRENT_BINARY_DIR}/Debug
# )