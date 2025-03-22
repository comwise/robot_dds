cmake_minimum_required(VERSION 3.2)

file(GLOB_RECURSE SOURCE_FILES "*.c*")

#add_library(node STATIC ${SOURCE_FILES})
#target_link_libraries(node srv pthread rt)

#add_executable(test1 test1.cc)
add_executable(test2 test2.cc)
if(WIN32)
    target_link_libraries(test2 ws2_32)
elseif(UNIX)
    target_link_libraries(test2 pthread)
endif()
