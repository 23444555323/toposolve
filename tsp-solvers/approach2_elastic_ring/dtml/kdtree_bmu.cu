#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include "../../common/types.hpp"

namespace tsp {
namespace approach2 {

__global__ void find_bmus_cuda_kernel(const float* nodes, int N, const float* ring, int M, int D, int* bmus) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < N) {
        float min_dist = 1e18f;
        int best_j = 0;
        float city_x = nodes[i * D];
        float city_y = nodes[i * D + 1];

        for (int j = 0; j < M; ++j) {
            float rx = ring[j * D];
            float ry = ring[j * D + 1];
            float dist = (city_x - rx) * (city_x - rx) + (city_y - ry) * (city_y - ry);
            if (dist < min_dist) {
                min_dist = dist;
                best_j = j;
            }
        }
        bmus[i] = best_j;
    }
}

void find_bmus_cuda(const float* nodes, int N, const float* ring, int M, int D, int* bmus) {
    float *d_nodes, *d_ring;
    int *d_bmus;

    CUDA_CHECK(cudaMalloc(&d_nodes, N * D * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&d_ring, M * D * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&d_bmus, N * sizeof(int)));

    CUDA_CHECK(cudaMemcpy(d_nodes, nodes, N * D * sizeof(float), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_ring, ring, M * D * sizeof(float), cudaMemcpyHostToDevice));

    int threads = 256;
    int blocks = (N + threads - 1) / threads;
    find_bmus_cuda_kernel<<<blocks, threads>>>(d_nodes, N, d_ring, M, D, d_bmus);

    CUDA_CHECK(cudaMemcpy(bmus, d_bmus, N * sizeof(int), cudaMemcpyDeviceToHost));

    CUDA_CHECK(cudaFree(d_nodes));
    CUDA_CHECK(cudaFree(d_ring));
    CUDA_CHECK(cudaFree(d_bmus));
}

} // namespace approach2
} // namespace tsp
