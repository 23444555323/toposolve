#include "ring_state.hpp"
#include <cufft.h>
#include <cuda_runtime.h>

namespace tsp {
namespace approach2 {

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
        // 1. Forward FFT: Rho -> Rho_hat
        cufftExecR2C(plan_fwd, d_density, d_freq);

        // 2. Multiply by Green's function in frequency domain: Phi_hat = Rho_hat * G_hat
        // G_hat(kx, ky) = -1 / (kx^2 + ky^2)
        // kernel_multiply_greens<<<...>>>(d_freq, grid_size);

        // 3. Inverse FFT: Phi_hat -> Phi (Potential)
        cufftExecC2R(plan_inv, d_freq, d_density);
    }

    ~FFTGrid() {
        cudaFree(d_density);
        cudaFree(d_freq);
        cufftDestroy(plan_fwd);
        cufftDestroy(plan_inv);
    }
};

} // namespace approach2
} // namespace tsp
