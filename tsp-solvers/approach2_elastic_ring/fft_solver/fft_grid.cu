#include "fft_grid.hpp"
#include <device_launch_parameters.h>

namespace tsp {
namespace approach2 {

__global__ void multiply_greens_kernel(cufftComplex* freq, int size) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < size && y < (size / 2 + 1)) {
        int idx = y * size + x;
        float kx = (x > size / 2) ? (float)(x - size) : (float)x;
        float ky = (float)y;
        float k_sq = kx * kx + ky * ky;

        if (k_sq > 1e-6f) {
            float g = -1.0f / k_sq;
            freq[idx].x *= g;
            freq[idx].y *= g;
        } else {
            freq[idx].x = 0;
            freq[idx].y = 0;
        }
    }
}

void FFTGrid::solve_poisson() {
    cufftExecR2C(plan_fwd, d_density.get(), d_freq.get());

    dim3 threads(16, 16);
    dim3 blocks((grid_size + threads.x - 1) / threads.x, (grid_size / 2 + 1 + threads.y - 1) / threads.y);
    multiply_greens_kernel<<<blocks, threads>>>(d_freq.get(), grid_size);

    cufftExecC2R(plan_inv, d_freq.get(), d_density.get());
}

} // namespace approach2
} // namespace tsp
