cmake_minimum_required(VERSION 3.21.0)

project(clone C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)