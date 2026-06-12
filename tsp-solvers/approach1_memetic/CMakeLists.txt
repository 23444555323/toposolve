# Approach 1 CMake
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

file(GLOB_RECURSE APPROACH1_SOURCES
    "pheromone_buffer/*.cpp"
    "aco/*.cu"
    "ga/*.cpp"
    "local_search/*.cpp"
    "local_search/*.cu"
)

add_library(approach1_memetic STATIC ${APPROACH1_SOURCES})
target_link_libraries(approach1_memetic PRIVATE OpenMP::OpenMP_CXX)
