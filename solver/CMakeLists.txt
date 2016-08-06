cmake_minimum_required(VERSION 3.0)
project(solver)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
        "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -frounding-math -ggdb -O0")

find_package(GMP REQUIRED)

find_package(Steven REQUIRED)

find_package(Boost 1.54 REQUIRED COMPONENTS program_options system filesystem)

include_directories("include" ${GMP_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS} ${Steven_INCLUDE_DIRS})

file(GLOB BACKWARD src/solver/*.cpp)
add_library(backward_solver SHARED ${BACKWARD})

file(GLOB SOURCE_FILES src/*.cpp)
add_executable(solver ${SOURCE_FILES})
target_link_libraries(solver backward_solver ${GMP_LIBRARIES} ${Boost_LIBRARIES} ${Steven_LIBRARIES})

add_subdirectory(tests)


# Convince clion to accept headers as part of the project by adding them to a dummy target
# Should not interfer with normal opratation.
FILE(GLOB_RECURSE clion_dummy_headers ${CMAKE_SOURCE_DIR}/*.hpp)
FILE(GLOB_RECURSE clion_dummy_source ${CMAKE_SOURCE_DIR}/src/main.cpp)
add_executable(clion_dummy_executable EXCLUDE_FROM_ALL ${clion_dummy_source} ${clion_dummy_headers})
