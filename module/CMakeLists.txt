cmake_minimum_required(VERSION 3.5)
project(module LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV 4.5.5 REQUIRED)

set(MODULE_SOURCE_FILES
        src/compute.cpp
        )

add_library(${PROJECT_NAME} SHARED
        ${MODULE_SOURCE_FILES}
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        include
        ${OpenCV_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME} PUBLIC
        ${OpenCV_LIBS}
        )
