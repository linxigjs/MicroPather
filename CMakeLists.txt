cmake_minimum_required(VERSION 3.5)
project(MicroPather)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
if (OPENCV_FOUND)
    message(STATUS "找到OpenCV: ${OpenCV_INCLUDE_DIRS}")
    message(STATUS "找到OpenCV: ${OpenCV_LIBS}")
    include_directories(${OpenCV_INCLUDE_DIRS})
endif (OPENCV_FOUND)

add_executable(Dungeon dungeon.cpp micropather.cpp micropather.h singlemap.h)

target_link_libraries(Dungeon ${OpenCV_LIBS} )