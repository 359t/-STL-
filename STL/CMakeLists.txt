cmake_minimum_required(VERSION 3.0)

project(stl)

set(CMAKE_BUILD_TYPE DEBUG)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include)



add_executable(main src/main.cpp include/stl.hpp) 

