cmake_minimum_required(VERSION 3.27)
project(
    SatFish
    VERSION 0.1
    DESCRIPTION "An improved version of the sat solver i wrote for my final year project at Swansea University"
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 22)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(apps)
