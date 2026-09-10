cmake_minimum_required(VERSION 3.20)
project(RubiksCubeSolver CXX)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenMP REQUIRED)

add_executable(RubiksCubeSolver
        main.cpp
        include/Cube.h
        src/DFS.cpp
        src/DFS.h
)

target_include_directories(RubiksCubeSolver PRIVATE include)

target_link_libraries(RubiksCubeSolver PRIVATE OpenMP::OpenMP_CXX)