cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(Stitch360)


################################
FIND_PACKAGE(gflags REQUIRED)
FIND_PACKAGE(glog REQUIRED)

# Add definitions

set(srcs
				Stitch360.cpp
)


################################

set(CPU_ARCH  ${CMAKE_SYSTEM_PROCESSOR})

if (MSVC)  
  if (CMAKE_SYSTEM_PROCESSOR MATCHES "(x86)|(X86)|(amd64)|(AMD64)")
      set (CPU_ARCH x64)
  else ()
      set (CPU_ARCH win32)
  endif ()
endif(MSVC)


link_directories("${CMAKE_SOURCE_DIR}/third_party/opencv/lib/${CPU_ARCH}")


################################
set(OPENCV_LIB 
              "opencv_core"
              "opencv_imgproc"
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


set(TARGET_DATA_COPY STITCH_COPY)
set(DATA_COPY_FILES)
foreach(data camera_rig.json cam0.png cam15.png cam_forward.png cam_backward.png)
  set(DATA_COPY_SRC  ${CMAKE_CURRENT_SOURCE_DIR}/resources/${data})
  set(DATA_COPY_DEST ${CMAKE_CURRENT_BINARY_DIR}/${data})
  add_custom_command(
    OUTPUT ${DATA_COPY_DEST}
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${DATA_COPY_SRC} ${DATA_COPY_DEST}
    DEPENDS ${DATA_COPY_SRC})
  list(APPEND DATA_COPY_FILES ${DATA_COPY_DEST})

endforeach(data)

add_custom_target(${TARGET_DATA_COPY} DEPENDS ${DATA_COPY_FILES})

add_executable(Stitch360 ${srcs})
add_dependencies(Stitch360 ${TARGET_DATA_COPY})


target_link_libraries( Stitch360
                       util
                       libPanoStitch
                       ${GFLAGS_LIBRARIES}
                       ${GLOG_LIBRARIES}
                       ${OPENCV_LIBRARY}
                       )     
                              
target_include_directories( Stitch360  PRIVATE
                       ${CMAKE_SOURCE_DIR}/third_party/
                       ${CMAKE_SOURCE_DIR}/third_party/opencv/include
)


