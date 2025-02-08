cmake_minimum_required(VERSION 3.5.0)
project(jack-compiler VERSION 0.1.0 LANGUAGES C CXX)

file(GLOB_RECURSE CXX_SOURCES "src/*.cpp")

add_executable(jack-compiler ${CXX_SOURCES})

