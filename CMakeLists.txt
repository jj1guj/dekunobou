cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
file(GLOB src src/**)

#最適化オプションの設定

#for Linux and Mac OS
set(CMAKE_CXX_FLAGS "-O2 -funroll-loops -mtune=native --std=c++17")

project(dekunobou)

add_executable(dekunobou ${src})
set_property(TARGET dekunobou PROPERTY CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
  target_link_libraries(dekunobou PUBLIC OpenMP::OpenMP_CXX)
endif()
