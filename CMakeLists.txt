cmake_minimum_required(VERSION 3.2) 

project(robot_dds)

set(CMAKE_CXX_FLAGS "-g -std=c++11")
#include(CheckCXXCompilerFlag)
#  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
#if(COMPILER_SUPPORTS_CXX11)
#  add_definitions("-std=c++11")
#endif()

include_directories(${PROJECT_SOURCE_DIR}/include/robot_dds)
include_directories(${PROJECT_SOURCE_DIR}/src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

link_directories(${PROJECT_SOURCE_DIR}/lib)

#enable_testing()
add_subdirectory(src)