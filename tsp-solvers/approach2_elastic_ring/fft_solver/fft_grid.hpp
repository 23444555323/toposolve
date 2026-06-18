#ifndef TSP_SOLVERS_FFT_GRID_HPP
#define TSP_SOLVERS_FFT_GRID_HPP

#include "approach2_elastic_ring/vse/ring_state.hpp"
#include <cufft.h>
#include <cuda_runtime.h>

namespace tsp {
namespace approach2 {

struct FFTGrid {
    cuda_unique_ptr<float> d_density;     // Rho: N x N grid
    cuda_unique_ptr<cufftComplex> d_freq; // Frequency domain
    cufftHandle plan_fwd;
    cufftHandle plan_inv;
    int grid_size;

    FFTGrid(int size) : grid_size(size) {
        float* density;
        cufftComplex* freq;
        CUDA_CHECK(cudaMalloc(&density, size * size * sizeof(float)));
        CUDA_CHECK(cudaMalloc(&freq, size * (size / 2 + 1) * sizeof(cufftComplex)));

        d_density.reset(density);
        d_freq.reset(freq);

        cufftPlan2d(&plan_fwd, size, size, CUFFT_R2C);
        cufftPlan2d(&plan_inv, size, size, CUFFT_C2R);
    }

    void solve_poisson();

    ~FFTGrid() {
        cufftDestroy(plan_fwd);
        cufftDestroy(plan_inv);
    }
};

} // namespace approach2
} // namespace tsp

#endif // TSP_SOLVERS_FFT_GRID_HPP
