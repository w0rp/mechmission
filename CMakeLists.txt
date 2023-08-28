cmake_minimum_required(VERSION 3.17)
project(mechmission)

# Require curses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-O3 -Wall -Wno-visibility -Wno-unused-variable")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(mechmission ${SOURCES})
# Link curses.
target_link_libraries(mechmission ${CURSES_LIBRARIES})
