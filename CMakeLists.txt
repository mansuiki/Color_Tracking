cmake_minimum_required(VERSION 3.14)
project(Color_Tracking)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenCV REQUIRED)

include_directories(.)
include_directories(data)

add_executable(Color_Tracking
        main.cpp)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})