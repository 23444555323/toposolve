#include "ring_state.hpp"

namespace tsp {
namespace approach2 {

__global__ void elastic_force_kernel(float* Y, float* forces, int M, int D, float beta, float K) {
    int j = blockIdx.x * blockDim.x + threadIdx.x;
    if (j < M) {
        for (int d = 0; d < D; ++d) {
            int curr = j * D + d;
            int next = ((j + 1) % M) * D + d;
            int prev = ((j - 1 + M) % M) * D + d;

            float f = beta * K * (Y[next] - 2 * Y[curr] + Y[prev]);
            atomicAdd(&forces[curr], f);
        }
    }
}

void compute_elastic_force(RingState& state, float beta, float K) {
    int threads = 256;
    int blocks = (state.M + threads - 1) / threads;
    elastic_force_kernel<<<blocks, threads>>>(state.d_Y, state.d_forces, state.M, state.D, beta, K);
}

} // namespace approach2
} // namespace tsp
