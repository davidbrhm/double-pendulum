cmake_minimum_required(VERSION 4.2)
project(double_pendulum C)

set(CMAKE_C_STANDARD 11)

find_package(raylib REQUIRED)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(double_pendulum ${SOURCES})

target_link_libraries(double_pendulum PRIVATE raylib)
