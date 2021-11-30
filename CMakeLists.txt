cmake_minimum_required(VERSION 3.20)
project(HashMap)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(HashMap
        main.cpp map_source.h map.h)
