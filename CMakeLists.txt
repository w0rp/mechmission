cmake_minimum_required(VERSION 3.17)
project(mechmission)

add_subdirectory(lib/entt)

# Require curses.
find_package(Curses REQUIRED)
# find_package(EnTT REQUIRED)

# Find every .cpp file to compile them all.
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

# Set the C++ standard and compiler options.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "\
-O3 -Wall -Wextra \
-Wshadow \
-Wno-unused-parameter \
-Wimplicit-fallthrough \
-Wconversion \
")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(mechmission ${SOURCES})

# Include entt and curses
# The libraries are marked as SYSTEM so we ignore warnings for them.
target_include_directories(
    mechmission
    SYSTEM
    PRIVATE
    lib/entt/src
    ${CURSES_INCLUDE_DIR}
)
# Link curses.
target_link_libraries(mechmission ${CURSES_LIBRARIES})
