#include "ring_state.hpp"
#include <device_launch_parameters.h>

namespace tsp {
namespace approach2 {

__device__ float sample_potential(const float* grid, float x, float y, int size) {
    int x0 = (int)floorf(x);
    int y0 = (int)floorf(y);
    float dx = x - x0;
    float dy = y - y0;

    auto get = [&](int i, int j) {
        i = (i + size) % size;
        j = (j + size) % size;
        return grid[j * size + i];
    };

    float v00 = get(x0, y0);
    float v10 = get(x0 + 1, y0);
    float v01 = get(x0, y0 + 1);
    float v11 = get(x0 + 1, y0 + 1);

    return (1 - dx) * (1 - dy) * v00 + dx * (1 - dy) * v10 + (1 - dx) * dy * v01 + dx * dy * v11;
}

__global__ void advective_force_fft_kernel(float* Y, float* forces, const float* potential_grid, int M, int D, int grid_size, float alpha) {
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    if (j < M) {
        float x = Y[j * D];
        float y = Y[j * D + 1];

        // Gradient via central differences of interpolated potential
        float eps = 0.1f;
        float p_xp = sample_potential(potential_grid, x + eps, y, grid_size);
        float p_xm = sample_potential(potential_grid, x - eps, y, grid_size);
        float p_yp = sample_potential(potential_grid, x, y + eps, grid_size);
        float p_ym = sample_potential(potential_grid, x, y - eps, grid_size);

        float fx = -alpha * (p_xp - p_xm) / (2 * eps);
        float fy = -alpha * (p_yp - p_ym) / (2 * eps);

        atomicAdd(&forces[j * D], fx);
        atomicAdd(&forces[j * D + 1], fy);
    }
}

} // namespace approach2
} // namespace tsp
