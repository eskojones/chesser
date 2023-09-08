cmake_minimum_required(VERSION 3.10)

project(chesser)

# Specify the required C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2 package
find_package(SDL2 REQUIRED)

# Collect all source files into a variable
file(GLOB SOURCES "*.c")

# Include header files directory
include_directories(include)

# Include SDL2 headers
include_directories(${SDL2_INCLUDE_DIRS})

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES}
        player.c
        player.h)

# Libraries to link
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
