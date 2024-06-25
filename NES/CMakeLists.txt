cmake_minimum_required(VERSION 3.8)
project(NES)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Change path from /src if needed, or add more directories
file(GLOB_RECURSE sources
        "${CMAKE_SOURCE_DIR}/src/*.c"
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        )
# Add precompiler definitions like that:
add_definitions(-DSOME_DEFINITION)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(NES.o ${sources})
target_link_libraries(NES.o ${SDL2_LIBRARIES})
