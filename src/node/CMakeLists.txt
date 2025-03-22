cmake_minimum_required(VERSION 3.2)

include_directories(node)

file(GLOB_RECURSE SOURCE_FILES "*.c*")

add_executable(robot_dds main.cpp)
#target_link_libraries(robot_dds node)