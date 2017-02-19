
set(source Sample.cpp)

add_definitions(-std=c++14)

add_executable(sample ${source})

target_link_libraries(sample pthread)