cmake_minimum_required(VERSION 3.6)
project(solver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file(GLOB SOURCE_FILES src/*.cpp)
add_executable(solver ${SOURCE_FILES})
