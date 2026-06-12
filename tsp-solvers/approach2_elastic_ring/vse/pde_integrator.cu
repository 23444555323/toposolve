#include "ring_state.hpp"

namespace tsp {
namespace approach2 {

__global__ void nesterov_pde_kernel(float* Y, float* V, float* forces, int M, int D, float dt, float mu) {
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    if (j < M) {
        for (int d = 0; d < D; ++d) {
            int idx = j * D + d;
            float v_old = V[idx];
            V[idx] = mu * V[idx] + dt * forces[idx];
            // Y_next = Y + V + mu * (V - V_old)
            Y[idx] += V[idx] + mu * (V[idx] - v_old);
            forces[idx] = 0.0f;
        }
    }
}

} // namespace approach2
} // namespace tsp
