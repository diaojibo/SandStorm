cmake_minimum_required(VERSION 3.9)
project(SandStorm)

set(CMAKE_CXX_STANDARD 11)


include_directories(./include/main)

add_executable(nimbus ./src/main/sandstorm/NimbusLauncher.cpp)