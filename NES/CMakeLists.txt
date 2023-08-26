cmake_minimum_required(VERSION 3.1)
project(NES)

file(GLOB SOURCES
    "${PROJECT_SOURCE_DIR}/*.cpp"
)

message(${PROJECT_SOURCE_DIR})

find_package(
  SFML 2
  COMPONENTS audio graphics window system
  REQUIRED)

add_executable(NES ${SOURCES})
