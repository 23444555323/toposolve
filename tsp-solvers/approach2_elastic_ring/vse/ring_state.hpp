#ifndef TSP_SOLVERS_RING_STATE_HPP
#define TSP_SOLVERS_RING_STATE_HPP

#include <cuda_runtime.h>
#include <vector>
#include <utility>
#include <memory>
#include "../../common/types.hpp"

namespace tsp {

struct CudaDeleter {
    template <typename T>
    void operator()(T* ptr) const {
        if (ptr) {
            CUDA_CHECK(cudaFree(ptr));
        }
    }
};

template<typename T>
using cuda_unique_ptr = std::unique_ptr<T[], CudaDeleter>;

namespace approach2 {

struct RingState {
    cuda_unique_ptr<float> d_Y;        // Ring node coordinates (M x D)
    cuda_unique_ptr<float> d_V;        // Velocities for Nesterov (M x D)
    cuda_unique_ptr<float> d_forces;   // Total forces (M x D)
    int M = 0;
    int D = 0;

    RingState() = default;
    RingState(int M, int D) : M(M), D(D) {
        float *y, *v, *f;
        CUDA_CHECK(cudaMalloc(&y, M * D * sizeof(float)));
        CUDA_CHECK(cudaMalloc(&v, M * D * sizeof(float)));
        CUDA_CHECK(cudaMalloc(&f, M * D * sizeof(float)));

        d_Y.reset(y);
        d_V.reset(v);
        d_forces.reset(f);

        CUDA_CHECK(cudaMemset(d_V.get(), 0, M * D * sizeof(float)));
        CUDA_CHECK(cudaMemset(d_forces.get(), 0, M * D * sizeof(float)));
    }

    RingState(RingState&&) noexcept = default;
    RingState& operator=(RingState&&) noexcept = default;

    RingState(const RingState&) = delete;
    RingState& operator=(const RingState&) = delete;

    float* get_Y() const { return d_Y.get(); }
    float* get_V() const { return d_V.get(); }
    float* get_forces() const { return d_forces.get(); }
};

} // namespace approach2
} // namespace tsp

#endif // TSP_SOLVERS_RING_STATE_HPP
