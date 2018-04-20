cmake_minimum_required(VERSION 2.8)
project(inpainting)

###############################################
#
# run following command on windows 
# cmake .. -G "Visual Studio 14 2015 Win64"
#
###############################################


message ("-- CMake Generator " ${CAMKE_GENERATOR})
set(CMAKE_BUILD_TYPE Debug)
message("-- Set build type: " ${CMAKE_BUILD_TYPE})

include_directories(${OpenCV_INCLUDE_DIRS} "inc")
message ("-- Add including directories: into search path." ${OpenCV_INCLUDE_DIRS} )

set (OpenCV_FIND_QUIETLY False)
message ("-- set  OpenCV_FIND_QUIETLY: "  ${OpenCV_FIND_QUIETLY})
	
	
if (WIN32)
	#set(OpenCV_STATIC true)
	message ("-- Win32 Platform ")
	set (OpenCV_DIR "c:/Users/ding/Documents/opencv/build/")
	message ("-- set  OpenCV_DIR: ${OpenCV_DIR}")
	
	#set (OpenCV_INCLUDE_DIRS "c:/Users/ding/Documents/opencv/build/include")
	#message ("-- set  OpenCV_INCLUDE_DIRS:  ${OpenCV_INCLUDE_DIRS}")
	
	add_definitions("-D_SCL_SECURE_NO_WARNINGS")
	
endif()

find_package(OpenCV REQUIRED)
message ("-- Opencv include dir: " ${OpenCV_INCLUDE_DIRS})
message ("-- Opencv OpenCV_STATIC: " ${OpenCV_STATIC})
message ("-- Opencv OpenCV_SHARED: " ${OpenCV_SHARED})
message ("-- Opencv OpenCV_LIBRARIES: " ${OpenCV_LIBRARIES})
message ("-- Opencv OpenCV_LIBS: "      ${OpenCV_LIBS})



if(UNIX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  message ("-- Enable c++11 on Unix like platform.")
endif(UNIX)

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
#target_link_libraries(inpaint ${OpenCV_LIBRARIES})
target_link_libraries(inpaint ${OpenCV_LIBRARIES})
	
# build Samples
add_executable(inpaint_image_criminisi examples/inpaint_image_criminisi.cpp)
target_link_libraries(inpaint_image_criminisi inpaint ${OpenCV_LIBRARIES})

#add_executable(patch_match examples/patch_match.cpp)
#target_link_libraries(patch_match inpaint ${OpenCV_LIBRARIES})
