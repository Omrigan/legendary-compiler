cmake_minimum_required(VERSION 3.2)
project(compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp LexicalAnalysis.cpp LexicalAnalysis.h enums.cpp)
add_executable(compiler ${SOURCE_FILES})