cmake_minimum_required(VERSION 3.22)

project(HopeItWork)

set(CMAKE_CXX_STANDARD 14)
#find_package(Qt5Widgets REQUIRED)
#target_link_libraries(HopeItWork Qt5::Widgets)

#add_executable(HopeItWork main.c LSB_Decode.c LSB_Encode.c lzw.c algorithms.c array.c file.c)
add_executable(HopeItWork main.c)
