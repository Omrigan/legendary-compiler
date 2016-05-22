cmake_minimum_required(VERSION 3.2)
project(compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
        lexical_analysis.h lexical_analysis.cpp
        syntax_analysis.h syntax_analysis.cpp
        lexem_types.h
        poliz.h
        Operations.cpp
        PolizQueue.cpp PolizQueue.h
        Generation.cpp Generation.h
        IdTable.cpp IdTable.h)
add_executable(compiler ${SOURCE_FILES})
