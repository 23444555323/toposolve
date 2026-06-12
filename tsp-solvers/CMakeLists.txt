cmake_minimum_required(VERSION 3.18)
project(tsp-solvers LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CUDA_STANDARD 17)
set(CMAKE_CUDA_SEPARABLE_COMPILATION ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} common)

add_subdirectory(approach1_memetic)
add_subdirectory(approach2_elastic_ring)

# Shared benchmark library
add_library(tsp_common common/benchmark.cpp)

# Example benchmark executable
add_executable(benchmark_main main.cpp)
target_link_libraries(benchmark_main PRIVATE approach1_memetic approach2_elastic_ring tsp_common)

# Verification Suite
find_package(CUFFT REQUIRED)
add_executable(verification_suite common/verification_suite.cu)
target_link_libraries(verification_suite PRIVATE approach1_memetic approach2_elastic_ring tsp_common cufft)
