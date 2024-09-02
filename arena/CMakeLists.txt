cmake_minimum_required(VERSION 3.21.0)

project(arena C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "-O3")

add_subdirectory(src)
target_link_libraries(arena m)