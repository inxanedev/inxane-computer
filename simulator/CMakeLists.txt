cmake_minimum_required(VERSION 3.10)
project(inxanecomputer-simulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ipcc-sim
    "src/main.cc"
    "src/runtime.cc"
)

target_include_directories(ipcc-sim PUBLIC "src")