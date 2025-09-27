cmake_minimum_required(VERSION 3.21)
project(CPE494 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external)

add_subdirectory(src/A1)
add_subdirectory(src/A2)
# add_subdirectory(src/Sandbox)
add_subdirectory(common)
