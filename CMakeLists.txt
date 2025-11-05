cmake_minimum_required(VERSION 4.0)
project(Blockchain)

set(CMAKE_CXX_STANDARD 20)

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -O3")

add_executable(Blockchain main.cpp hash.hpp)