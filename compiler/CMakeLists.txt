cmake_minimum_required(VERSION 3.10)
project(inxanecomputer-compiler)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_executable(ipcc
    "src/main.cc"
    "src/source_reader.cc"
    "src/parser.cc"
    "src/compiler.cc"
)

target_include_directories(ipcc PUBLIC "src")