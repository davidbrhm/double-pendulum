cmake_minimum_required(VERSION 4.2)
project(double_pendulum C)

set(CMAKE_C_STANDARD 11)

find_package(raylib REQUIRED)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(double_pendulum ${SOURCES}
        include/ui.h
        include/constants.h
        include/physics.h
        include/render.h
        include/logger.h
        include/screens/screen_menu.h
        include/screens/screen_core_sim.h
        include/screens/screen_settings.h
        src/screens/screen_butterfly_effect.c
        include/screens/screen_butterfly_effect.h
)

target_link_libraries(double_pendulum PRIVATE raylib)
