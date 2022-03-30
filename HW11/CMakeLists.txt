# 
# homeowork11/CMakeLists.txt --
#

cmake_minimum_required(VERSION 3.12)

project(HW11 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
  add_compile_options(-Wall -Wextra -pedantic -Werror)
  add_compile_definitions(FORTIFY_SOURCE=2)
endif()

add_executable(catloop catloop.c)
