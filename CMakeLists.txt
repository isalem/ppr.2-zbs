cmake_minimum_required(VERSION 3.3)
project(zbs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build/")

find_package(GMP REQUIRED)
find_package(MPI REQUIRED)
find_package(Tclap REQUIRED)

set(CMAKE_CXX_COMPILE_FLAGS ${CMAKE_CXX_COMPILE_FLAGS} ${MPI_COMPILE_FLAGS})
set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS} ${MPI_LINK_FLAGS})

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${GMP_INCLUDE_DIR})
include_directories(${TCLAP_INCLUDE_DIR})
include_directories(MPI_INCLUDE_PATH)

set(SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/main.cpp
        ${PROJECT_SOURCE_DIR}/src/matrix.cpp)
add_executable(zbs ${SOURCE_FILES})

target_link_libraries(zbs ${GMP_LIBRARIES})
target_link_libraries(zbs ${MPI_LIBRARIES})
