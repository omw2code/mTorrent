cmake_minimum_required(VERSION 3.16)
project(mTorrent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Required libraries
find_package(OpenSSL REQUIRED)
find_package(nlohmann_json REQUIRED)
set(JSON_ImplicitConversions OFF)

# Set up general include for generator expressions
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Traverse the rest of the project
add_subdirectory(external)
add_subdirectory(src)
add_subdirectory(test)
