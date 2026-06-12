#ifndef TSP_SOLVERS_RING_STATE_HPP
#define TSP_SOLVERS_RING_STATE_HPP

#include <cuda_runtime.h>
#include <vector>
#include <utility>

namespace tsp {
namespace approach2 {

struct RingState {
    float* d_Y = nullptr;        // Ring node coordinates (M x D)
    float* d_V = nullptr;        // Velocities for Nesterov (M x D)
    float* d_forces = nullptr;   // Total forces (M x D)
    int M = 0;
    int D = 0;

    RingState() = default;
    RingState(int M, int D) : M(M), D(D) {
        cudaMalloc(&d_Y, M * D * sizeof(float));
        cudaMalloc(&d_V, M * D * sizeof(float));
        cudaMalloc(&d_forces, M * D * sizeof(float));
        cudaMemset(d_V, 0, M * D * sizeof(float));
        cudaMemset(d_forces, 0, M * D * sizeof(float));
    }

    ~RingState() {
        if (d_Y) cudaFree(d_Y);
        if (d_V) cudaFree(d_V);
        if (d_forces) cudaFree(d_forces);
    }

    // Delete copy operations
    RingState(const RingState&) = delete;
    RingState& operator=(const RingState&) = delete;

    // Default move operations
    RingState(RingState&& other) noexcept
        : d_Y(std::exchange(other.d_Y, nullptr)),
          d_V(std::exchange(other.d_V, nullptr)),
          d_forces(std::exchange(other.d_forces, nullptr)),
          M(other.M), D(other.D) {}

    RingState& operator=(RingState&& other) noexcept {
        if (this != &other) {
            if (d_Y) cudaFree(d_Y);
            if (d_V) cudaFree(d_V);
            if (d_forces) cudaFree(d_forces);
            d_Y = std::exchange(other.d_Y, nullptr);
            d_V = std::exchange(other.d_V, nullptr);
            d_forces = std::exchange(other.d_forces, nullptr);
            M = other.M;
            D = other.D;
        }
        return *this;
    }
};

} // namespace approach2
} // namespace tsp

#endif // TSP_SOLVERS_RING_STATE_HPP
