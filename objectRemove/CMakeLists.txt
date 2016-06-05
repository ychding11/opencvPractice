cmake_minimum_required(VERSION 2.8)
project(inpainting)

set(CMAKE_BUILD_TYPE Debug)
message("-- Set build type: " ${CMAKE_BUILD_TYPE})

if (WIN32)
	#set(OpenCV_STATIC true)
	add_definitions("-D_SCL_SECURE_NO_WARNINGS")
endif()

if(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  message ("-- Enable c++11")
endif(UNIX)

find_package(OpenCV REQUIRED)
message ("-- Opencv includes dir " ${OpenCV_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS} "inc")
message ("-- Add including directories into search path.")

# Build Library
add_library(inpaint 
	inc/inpaint/stats.h
	inc/inpaint/patch.h
	inc/inpaint/gradient.h
    inc/inpaint/integral.h
    inc/inpaint/timer.h
	inc/inpaint/criminisi_inpainter.h
	inc/inpaint/template_match_candidates.h
	inc/inpaint/patch_match.h
	src/criminisi_inpainter.cpp
	src/template_match_candidates.cpp
	src/patch_match.cpp
)
target_link_libraries(inpaint ${OpenCV_LIBRARIES})
	
# Samples

add_executable(inpaint_image_criminisi examples/inpaint_image_criminisi.cpp)
target_link_libraries(inpaint_image_criminisi inpaint ${OpenCV_LIBRARIES})

add_executable(patch_match examples/patch_match.cpp)
target_link_libraries(patch_match inpaint ${OpenCV_LIBRARIES})
