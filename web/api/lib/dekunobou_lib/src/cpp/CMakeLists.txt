cmake_minimum_required(VERSION 3.10)
project("dekunobou for Rust" LANGUAGES CXX C)
file(GLOB src ../../../../../../src/**)
list(FILTER src EXCLUDE REGEX ".*main.cpp")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fopenmp -O2 -funroll-loops -mtune=native --std=c++17")

set(TARGET dekunobou)

# 必ずstaticライブラリにビルドすること
add_library(${TARGET}
  STATIC
    dekunobou.cpp
    ${src}
)

install (TARGETS ${TARGET} DESTINATION .)