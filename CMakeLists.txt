cmake_minimum_required(VERSION 3.20)
project(CppFun)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Set up general include for generator expressions
add_library(include INTERFACE)
target_include_directories(include
  INTERFACE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)

# Traverse the rest of the project
add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(test)
