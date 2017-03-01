
cmake_minimum_required(VERSION 3.0)

set(project sample)

set(source Sample.cpp)

add_definitions(-std=c++14)

add_executable(${project} ${source})

target_link_libraries(${project} pthread)