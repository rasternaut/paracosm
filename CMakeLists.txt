
# Mandatory CMake Boilerplate
cmake_minimum_required(VERSION 3.24)

project( paracosm VERSION 0.0
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(ThirdParty.cmake)

add_subdirectory(src)
