cmake_minimum_required(VERSION 3.3)
project(zbs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES ./src/main.cpp)
add_executable(zbs ${SOURCE_FILES})