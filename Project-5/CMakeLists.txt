cmake_minimum_required(VERSION 3.21)
project(P5)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(P5 src/z5.c src/functions.c src/config.h)
target_link_libraries(P5 m)
