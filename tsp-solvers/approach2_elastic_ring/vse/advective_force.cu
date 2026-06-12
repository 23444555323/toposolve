#include "ring_state.hpp"

namespace tsp {
namespace approach2 {

__global__ void advective_force_fft_kernel(float* Y, float* forces, float* potential_grid, int M, int D, int grid_size) {
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    if (j < M) {
        // Sample gradient of potential field at ring node j
        // F_adv = -grad(Phi)
        float x = Y[j * D];
        float y = Y[j * D + 1];

        // Bi-linear interpolation from potential_grid
        // float fx = sample_grad_x(potential_grid, x, y, grid_size);
        // float fy = sample_grad_y(potential_grid, x, y, grid_size);

        // atomicAdd(&forces[j * D], fx);
        // atomicAdd(&forces[j * D + 1], fy);
    }
}

} // namespace approach2
} // namespace tsp
