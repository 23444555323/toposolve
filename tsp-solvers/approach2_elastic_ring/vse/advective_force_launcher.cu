#include "ring_state.hpp"
#include <device_launch_parameters.h>

namespace tsp {
namespace approach2 {

__global__ void advective_force_fft_kernel(float* Y, float* forces, const float* potential_grid, int M, int D, int grid_size, float alpha);

void launch_advective_force_fft(float* Y, float* forces, const float* potential_grid, int M, int D, int grid_size, float alpha) {
    int threads = 256;
    int blocks = (M + threads - 1) / threads;
    advective_force_fft_kernel<<<blocks, threads>>>(Y, forces, potential_grid, M, D, grid_size, alpha);
}

} // namespace approach2
} // namespace tsp
