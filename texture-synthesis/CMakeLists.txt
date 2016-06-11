cmake_minimum_required(VERSION 2.8)
project( texture-synthesis )

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

find_package( OpenCV REQUIRED )

add_executable(foregroundRemoval  foregroundRemoval.cpp)
target_link_libraries (foregroundRemoval ${OpenCV_LIBS})
