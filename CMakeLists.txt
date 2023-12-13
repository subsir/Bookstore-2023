cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
file(GLOB SOURCES "src/*.cpp")

add_executable(code ${SOURCES})