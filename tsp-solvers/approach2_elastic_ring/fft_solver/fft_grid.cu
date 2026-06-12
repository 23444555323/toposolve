#include <cufft.h>
#include <cuda_runtime.h>

namespace tsp {
namespace approach2 {

struct FFTGrid {
    float* d_grid;
    cufftHandle plan_fwd;
    cufftHandle plan_inv;
    int grid_size;

    FFTGrid(int size) : grid_size(size) {
        cudaMalloc(&d_grid, size * size * sizeof(float));
        // Initialize cuFFT plans
        cufftPlan2d(&plan_fwd, size, size, CUFFT_R2C);
        cufftPlan2d(&plan_inv, size, size, CUFFT_C2R);
    }

    ~FFTGrid() {
        cudaFree(d_grid);
        cufftDestroy(plan_fwd);
        cufftDestroy(plan_inv);
    }
};

} // namespace approach2
} // namespace tsp
