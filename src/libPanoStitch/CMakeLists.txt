cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(libPanoStitch)


#CMAKE_BUILD_TOOL

################################
# set lib version here

set(GENERIC_LIB_VERSION "1.0.0")
set(GENERIC_LIB_SOVERSION "1")

#set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")


### LibJSON ###

ADD_LIBRARY(
  LibJSON
  thirdparty/supereasyjson/json.cpp
)


find_package(Eigen3 REQUIRED)
find_package(Ceres REQUIRED)
find_package(glog)


INCLUDE_DIRECTORIES(${EIGEN_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../util)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/calibration)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/optical_flow)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/render)

set(CPU_ARCH  ${CMAKE_SYSTEM_PROCESSOR})

if (MSVC)  
	if(NOT "${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
      set (CPU_ARCH x86)
  else ()
      set (CPU_ARCH x64)
  endif ()
endif(MSVC)


link_directories("${CMAKE_SOURCE_DIR}/third_party/opencv/lib/${CPU_ARCH}")

# Add targets


FILE(GLOB render_SRC "render/*.cpp")
FILE(GLOB optical_flow_SRC "optical_flow/*.cpp")
FILE(GLOB calibration_SRC "calibration/*.cpp")

set(srcs IPanoStitch.h
		libPanoStitch.cpp libPanoStitch.h
    PanoStitchBase.cpp PanoStitchBase.h
    V10PanoStitch.cpp V10PanoStitch.h
    CalibraterBase.cpp CalibraterBase.h
    V10Calibrater.cpp V10Calibrater.h
    ${render_SRC}
    ${optical_flow_SRC}
    ${calibration_SRC}
)

################################
set(OPENCV_LIB "opencv_calib3d"
              "opencv_core" "opencv_features2d"
              "opencv_flann" "opencv_highgui" "opencv_imgcodecs"
              "opencv_imgproc" "opencv_ml" "opencv_shape"
              "opencv_video" "opencv_videoio" "opencv_xfeatures2d"
              "opencv_objdetect"   "opencv_stitching"
              )

IF(NOT ${CMAKE_BUILD_TYPE} MATCHES Debug)
  set(OPENCV_VERSION "320")
ELSE ()              
  set(OPENCV_VERSION "320d")
ENDIF()

set(OPENCV_LIBRARY "")
foreach(lib ${OPENCV_LIB})
   list(APPEND OPENCV_LIBRARY ${lib}${OPENCV_VERSION})
endforeach(lib)

IF(NOT ${CMAKE_BUILD_TYPE} MATCHES Debug)
  list(APPEND OPENCV_LIBRARY "libjpeg")
  list(APPEND OPENCV_LIBRARY "libpng")
  list(APPEND OPENCV_LIBRARY "zlib")
ELSE ()              
  list(APPEND OPENCV_LIBRARY "libjpegd")
  list(APPEND OPENCV_LIBRARY "libpngd")
  list(APPEND OPENCV_LIBRARY "zlibd")
ENDIF()

#IF(NOT ${CMAKE_BUILD_TYPE} MATCHES Debug)
#set(OPENCV_LIBRARY "opencv_calib3d320"
#              "opencv_core320" "opencv_features2d320"
#              "opencv_flann320" "opencv_highgui320" "opencv_imgcodecs320"
#              "opencv_imgproc320" "opencv_ml320" "opencv_shape320"
#              "opencv_video320" "opencv_videoio320" "opencv_xfeatures2d320"
#              "zlibd"
#              )
#ELSE ()              
#set(OPENCV_LIBRARY "opencv_calib3d320d"
#              "opencv_core320d" "opencv_features2d320d"
#              "opencv_flann320d" "opencv_highgui320d" "opencv_imgcodecs320d"
#              "opencv_imgproc320d" "opencv_ml320d" "opencv_shape320d"
#              "opencv_video320d" "opencv_videoio320d" "opencv_xfeatures2d320d"
#              "zlib"
#              )
#ENDIF()

add_library(libPanoStitch SHARED ${srcs})


target_include_directories(libPanoStitch PRIVATE 
                ${CMAKE_SOURCE_DIR}/third_party
                ${CMAKE_SOURCE_DIR}/third_party/opencv/include
                ${CMAKE_SOURCE_DIR}/third_party/boost
                ${GLOG_INCLUDE_DIR}
                ${CERES_INCLUDE_DIR}
            )
target_link_libraries( # Specifies the target library.
                       libPanoStitch
                       util
                       LibJSON
                       ${GLOG_LIBRARIES}
                       ${EIGEN3_LIBRARIES}
                       ${CERES_LIBRARIES}
                       ${OPENCV_LIBRARY}

                       )            
            

target_compile_definitions(libPanoStitch PRIVATE 
            _USE_MATH_DEFINES LIBPANOSTITCH_EXPORTS)

if(NOT MSVC)  
	set_target_properties( libPanoStitch PROPERTIES OUTPUT_NAME PanoStitch )
endif(NOT MSVC)
