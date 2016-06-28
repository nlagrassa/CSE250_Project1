cmake_minimum_required(VERSION 3.5)
project(CSE250_Project1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES UBCalcDriver.cpp UBCalculator.cpp error_handling.cpp Lexer.cpp)
add_executable(CSE250_Project1 ${SOURCE_FILES})


