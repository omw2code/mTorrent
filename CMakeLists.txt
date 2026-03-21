cmake_minimum_required(VERSION 3.20)
project(CppFun)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(test)
