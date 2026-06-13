#include "ring_state.hpp"
#include <device_launch_parameters.h>

namespace tsp {
namespace approach2 {

__global__ void advective_force_fft_kernel(float* Y, float* forces, cudaTextureObject_t potentialTex, int M, int D, int grid_size, float alpha) {
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    if (j < M) {
        float x = Y[j * D];
        float y = Y[j * D + 1];

        float eps = 0.1f;
        float p_xp = tex2D<float>(potentialTex, x + eps, y);
        float p_xm = tex2D<float>(potentialTex, x - eps, y);
        float p_yp = tex2D<float>(potentialTex, x, y + eps);
        float p_ym = tex2D<float>(potentialTex, x, y - eps);

        float fx = -alpha * (p_xp - p_xm) / (2 * eps);
        float fy = -alpha * (p_yp - p_ym) / (2 * eps);

        atomicAdd(&forces[j * D], fx);
        atomicAdd(&forces[j * D + 1], fy);
    }
}

void launch_advective_force_fft(float* Y, float* forces, cudaTextureObject_t potentialTex, int M, int D, int grid_size, float alpha) {
    int threads = 256;
    int blocks = (M + threads - 1) / threads;
    advective_force_fft_kernel<<<blocks, threads>>>(Y, forces, potentialTex, M, D, grid_size, alpha);
}

} // namespace approach2
} // namespace tsp
