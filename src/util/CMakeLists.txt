cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(util)


#CMAKE_BUILD_TOOL

################################
# set lib version here

set(GENERIC_LIB_VERSION "1.0.0")
set(GENERIC_LIB_SOVERSION "1")


find_package(glog)
find_package(gflags)

INCLUDE_DIRECTORIES(${glog_DIR}/../../../include)
INCLUDE_DIRECTORIES(${gflags_INCLUDE_DIR})

# Add targets


FILE(GLOB util_SRC "./*.cpp")

set(srcs 
    ${util_SRC}    
)


add_library(util STATIC ${srcs})

target_include_directories( util  PRIVATE
                       ${CMAKE_SOURCE_DIR}/third_party/opencv/include
)
target_compile_definitions(util PRIVATE 
            _USE_MATH_DEFINES)


