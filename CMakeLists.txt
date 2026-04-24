cmake_minimum_required(VERSION 3.10)
project(ProtoLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_library(protolib
    src/buffer.cpp
    src/vector_store.cpp
)

add_executable(demo
    examples/demo.cpp
)

target_link_libraries(demo protolib)