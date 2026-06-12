#include "ring_state.hpp"
#include <cufft.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

namespace tsp {
namespace approach2 {

__global__ void multiply_greens_kernel(cufftComplex* freq, int size);

struct FFTGrid {
    float* d_density;     // Rho: N x N grid
    cufftComplex* d_freq; // Frequency domain
    cufftHandle plan_fwd;
    cufftHandle plan_inv;
    int grid_size;

    FFTGrid(int size) : grid_size(size) {
        cudaMalloc(&d_density, size * size * sizeof(float));
        cudaMalloc(&d_freq, size * (size / 2 + 1) * sizeof(cufftComplex));

        cufftPlan2d(&plan_fwd, size, size, CUFFT_R2C);
        cufftPlan2d(&plan_inv, size, size, CUFFT_C2R);
    }

    void solve_poisson() {
        cufftExecR2C(plan_fwd, d_density, d_freq);

        dim3 threads(16, 16);
        dim3 blocks((grid_size + threads.x - 1) / threads.x, (grid_size / 2 + 1 + threads.y - 1) / threads.y);
        multiply_greens_kernel<<<blocks, threads>>>(d_freq, grid_size);

        cufftExecC2R(plan_inv, d_freq, d_density);
    }

    ~FFTGrid() {
        if (d_density) cudaFree(d_density);
        if (d_freq) cudaFree(d_freq);
        cufftDestroy(plan_fwd);
        cufftDestroy(plan_inv);
    }
};

} // namespace approach2
} // namespace tsp
