cmake_minimum_required(VERSION 3.18)
project(tsp-solvers LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CUDA_STANDARD 17)

include_directories(common)

add_subdirectory(approach1_memetic)
add_subdirectory(approach2_elastic_ring)

# Shared benchmark library
add_library(tsp_common common/benchmark.cpp)

# Example benchmark executable
add_executable(benchmark_main main.cpp)
target_link_libraries(benchmark_main PRIVATE approach1_memetic approach2_elastic_ring tsp_common)
