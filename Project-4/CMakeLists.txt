cmake_minimum_required(VERSION 3.21)
project(ok C)

set(CMAKE_C_STANDARD 99)

add_executable(ok z4.c functions.c data.c config.h)
