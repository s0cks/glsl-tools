cmake_minimum_required(VERSION 3.3)
project("glsl-tools")

set(CMAKE_CXX_STANDARD 11)
file(GLOB_RECURSE HEADERS Sources/*.h)
file(GLOB_RECURSE SOURCES Sources/*.cc)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
