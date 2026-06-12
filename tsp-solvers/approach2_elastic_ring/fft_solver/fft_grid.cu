#include "fft_grid.hpp"
#include <device_launch_parameters.h>

namespace tsp {
namespace approach2 {

__global__ void multiply_greens_kernel(cufftComplex* freq, int size) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x < size && y < (size / 2 + 1)) {
        int idx = y * size + x;
        float kx = (x > size / 2) ? (float)(x - size) : (float)x;
        float ky = (float)y;
        float k_sq = kx * kx + ky * ky;

        if (k_sq > 1e-6f) {
            float g = -1.0f / k_sq;
            freq[idx].x *= g;
            freq[idx].y *= g;
        } else {
            freq[idx].x = 0;
            freq[idx].y = 0;
        }
    }
}

} // namespace approach2
} // namespace tsp
