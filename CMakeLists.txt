cmake_minimum_required(VERSION 3.5)
project(CSE250_Project 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES mainClass.cpp)
add_executable(CSE250_Project 1 ${SOURCE_FILES})