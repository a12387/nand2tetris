cmake_minimum_required(VERSION 3.13)
project(hack-assembler)

set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)
file(GLOB_RECURSE CXX_SOURCES "src/*.cpp")
add_executable(assembler ${CXX_SOURCES})