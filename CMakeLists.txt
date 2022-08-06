cmake_minimum_required (VERSION 3.1)
project (alpha4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 23)
add_compile_options(-Wall -Wpedantic -Wextra -Werror)

add_subdirectory(src/)

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
add_compile_options(-Og -O0)
else()
  add_compile_options(-O3)
endif()