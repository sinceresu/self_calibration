cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(Stitch360)


# Add definitions


if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++14 -mavx2 -Wdeprecated-declarations  -Wall -Wreorder")
endif()

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/third_party)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libPanoStitch)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/util)

add_subdirectory("util/")
add_subdirectory("libPanoStitch/")
add_subdirectory("Stitch360/")
add_subdirectory("Calibrate360/")
################################









