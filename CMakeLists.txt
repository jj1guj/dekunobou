cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
file(GLOB src src/**)

#最適化オプションの設定

#for Linux
set(CMAKE_CXX_FLAGS "-fopenmp -O2 -funroll-loops -mtune=native --std=c++17")

project(dekunobou-new)

add_executable(dekunobou ${src})
set_property(TARGET dekunobou PROPERTY CXX_STANDARD 17)