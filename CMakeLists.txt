cmake_minimum_required(VERSION 3.10)
project(ProtoLib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(ENABLE_OPENMP "Enable OpenMP parallelization" ON)

if(ENABLE_ASAN)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif()

include_directories(include)

add_library(protolib_core
    src/buffer.cpp
    src/vector_store.cpp
    src/distance.cpp
)
set_target_properties(protolib_core PROPERTIES POSITION_INDEPENDENT_CODE ON)

if(ENABLE_OPENMP)
    find_package(OpenMP)
    if(OpenMP_CXX_FOUND)
        target_link_libraries(protolib_core PUBLIC OpenMP::OpenMP_CXX)
    endif()
endif()

add_executable(demo examples/demo.cpp)
target_link_libraries(demo protolib_core)

add_executable(test_buffer tests/test_buffer.cpp)
target_link_libraries(test_buffer protolib_core)

add_executable(test_vector_store tests/test_vector_store.cpp)
target_link_libraries(test_vector_store protolib_core)

# GTest
enable_testing()
find_package(GTest REQUIRED)

add_executable(test_buffer_gtest tests/test_buffer_gtest.cpp)
target_link_libraries(test_buffer_gtest protolib_core GTest::GTest GTest::Main)

add_executable(test_vector_store_gtest tests/test_vector_store_gtest.cpp)
target_link_libraries(test_vector_store_gtest protolib_core GTest::GTest GTest::Main)

add_test(NAME BufferGTest COMMAND test_buffer_gtest)
add_test(NAME VectorStoreGTest COMMAND test_vector_store_gtest)

add_executable(test_distance_gtest tests/test_distance_gtest.cpp)
target_link_libraries(test_distance_gtest protolib_core GTest::GTest GTest::Main)

add_test(NAME DistanceGTest COMMAND test_distance_gtest)

add_executable(test_pipeline_gtest tests/test_pipeline_gtest.cpp)
target_link_libraries(test_pipeline_gtest protolib_core GTest::GTest GTest::Main)

add_test(NAME PipelineGTest COMMAND test_pipeline_gtest)

add_executable(benchmark_search benchmarks/benchmark_search.cpp)
target_link_libraries(benchmark_search protolib_core)

# Python module
find_package(pybind11 REQUIRED)

pybind11_add_module(protolib python/binding.cpp)
target_link_libraries(protolib PRIVATE protolib_core)
