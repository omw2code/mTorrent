cmake_minimum_required(VERSION 3.20)
project(mTorrent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Required libraries
find_package(OpenSSL REQUIRED)

# Set up general include for generator expressions
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Traverse the rest of the project
add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(test)
