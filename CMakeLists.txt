cmake_minimum_required(VERSION 3.3)
project(zbs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Tclap REQUIRED)
include_directories(${TCLAP_INCLUDE_DIR})

set(SOURCE_FILES ./src/main.cpp)
add_executable(zbs ${SOURCE_FILES})