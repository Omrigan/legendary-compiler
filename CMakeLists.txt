cmake_minimum_required(VERSION 3.2)
project(compiler)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp  syntax_analysis.h lexical_analysis.h syntax_analysis.h lexical_analysis.h syntax_analysis.cpp lexical_analysis.cpp lexem_types.cpp Operands.cpp Operations.cpp PolizQueue.cpp PolizQueue.h IdTable.cpp IdTable.h Operands.h Operations.h)
add_executable(compiler ${SOURCE_FILES})
