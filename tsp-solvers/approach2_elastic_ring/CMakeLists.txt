# Approach 2 CMake
find_package(CUFFT REQUIRED)

file(GLOB_RECURSE APPROACH2_SOURCES
    "vse/*.cu"
    "fft_solver/*.cu"
    "dtml/*.cpp"
    "dtml/*.cu"
)

add_library(approach2_elastic_ring STATIC ${APPROACH2_SOURCES})
target_link_libraries(approach2_elastic_ring PRIVATE cufft)
