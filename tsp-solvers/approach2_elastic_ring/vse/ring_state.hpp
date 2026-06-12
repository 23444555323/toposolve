#ifndef TSP_SOLVERS_RING_STATE_HPP
#define TSP_SOLVERS_RING_STATE_HPP

#include <cuda_runtime.h>
#include <vector>

namespace tsp {
namespace approach2 {

struct RingState {
    float* d_Y;        // Ring node coordinates (M x D)
    float* d_V;        // Velocities for Nesterov (M x D)
    float* d_forces;   // Total forces (M x D)
    int M;
    int D;

    RingState(int M, int D) : M(M), D(D) {
        cudaMalloc(&d_Y, M * D * sizeof(float));
        cudaMalloc(&d_V, M * D * sizeof(float));
        cudaMalloc(&d_forces, M * D * sizeof(float));
        cudaMemset(d_V, 0, M * D * sizeof(float));
    }

    ~RingState() {
        cudaFree(d_Y);
        cudaFree(d_V);
        cudaFree(d_forces);
    }
};

} // namespace approach2
} // namespace tsp

#endif // TSP_SOLVERS_RING_STATE_HPP
