#include "ring_state.hpp"

namespace tsp {
namespace approach2 {

__global__ void pde_step_kernel(float* Y, float* V, float* forces, int M, int D, float dt) {
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    if (j < M) {
        for (int d = 0; d < D; ++d) {
            int idx = j * D + d;
            V[idx] = 0.9f * V[idx] + 0.1f * forces[idx];
            Y[idx] += V[idx] * dt;
            forces[idx] = 0.0f; // Reset for next iteration
        }
    }
}

} // namespace approach2
} // namespace tsp
