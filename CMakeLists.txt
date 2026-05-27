cmake_minimum_required(VERSION 3.10)
project(ProtoLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)

if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

include_directories(include)

add_library(protolib
    src/buffer.cpp
    src/vector_store.cpp
)

add_executable(demo examples/demo.cpp)
target_link_libraries(demo protolib)

add_executable(test_buffer tests/test_buffer.cpp)
target_link_libraries(test_buffer protolib)

add_executable(test_vector_store tests/test_vector_store.cpp)
target_link_libraries(test_vector_store protolib)

# GTest
enable_testing()
find_package(GTest REQUIRED)

add_executable(test_buffer_gtest tests/test_buffer_gtest.cpp)
target_link_libraries(test_buffer_gtest protolib GTest::GTest GTest::Main)

add_executable(test_vector_store_gtest tests/test_vector_store_gtest.cpp)
target_link_libraries(test_vector_store_gtest protolib GTest::GTest GTest::Main)

add_test(NAME BufferGTest COMMAND test_buffer_gtest)
add_test(NAME VectorStoreGTest COMMAND test_vector_store_gtest)
