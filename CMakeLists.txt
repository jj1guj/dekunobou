cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
file(GLOB src src/**)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_AR llvm-ar)
set(CMAKE_RANLIB llvm-ranlib)

project(dekunobou)

add_executable(dekunobou ${src})
set_property(TARGET dekunobou PROPERTY CXX_STANDARD 17)

# for Linux and Mac OS
target_compile_options(dekunobou PRIVATE
  -O3 -Ofast -march=native -mtune=native -flto=thin
  -ffast-math -fassociative-math -freciprocal-math 
  -funroll-loops
  -fomit-frame-pointer
  -mavx2 -mfma
)

target_link_options(dekunobou PRIVATE -flto=thin -fuse-ld=lld)

find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
  target_link_libraries(dekunobou PUBLIC OpenMP::OpenMP_CXX)
endif()
