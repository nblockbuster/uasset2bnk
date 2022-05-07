# CMakeList.txt : CMake project for ua2bnk, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 17)
project ("ua2bnk")

# Add source to this project's executable.
add_executable (ua2bnk "ua2bnk.cpp" "ua2bnk.h")

# TODO: Add tests and install targets if needed.
