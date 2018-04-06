cmake_minimum_required(VERSION 3.8)
project(Demonstration_Implementation)

set(CMAKE_CXX_STANDARD 98)

set(SOURCE_FILES old_main.cpp main.cpp heartbeat.h heartbeat.cpp heartbeat_struct.cpp heartbeat_struct.h)
add_executable(Demonstration_Implementation ${SOURCE_FILES})
