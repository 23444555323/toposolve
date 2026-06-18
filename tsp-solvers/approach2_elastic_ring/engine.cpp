#include "engine.hpp"
#include "vse/ring_state.hpp"
#include "fft_solver/fft_grid.hpp"
#include "vse/annealing.cu"
#include "dtml/dtml.hpp"
#include <cmath>
#include <numeric>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace tsp {
namespace approach2 {

// Forward declarations
void compute_elastic_force(RingState& state, float beta, float K);
void launch_advective_force_fft(float* Y, float* forces, cudaTextureObject_t potentialTex, int M, int D, int grid_size, float alpha);
void launch_nesterov_pde(float* Y, float* V, float* forces, int M, int D, float dt, float mu);
void find_bmus_cuda(const float* nodes, int N, const float* ring, int M, int D, int* bmus);
void deduplicate_and_cleanup(std::vector<int>& tour, const Graph& g);

Tour ElasticEngine::solve(const ProblemInstance& instance, const SolverConfig& config) {
    int N = instance.size();
    int M = static_cast<int>(config.gamma * N);
    int D = 2;
    int grid_size = 256;

    RingState state(M, D);
    FFTGrid grid(grid_size);
    AnnealingSchedule schedule { static_cast<float>(config.K0), static_cast<float>(config.epsilon) };

    // 1. Density projection (Discretize city coordinates onto grid)
    std::vector<float> density_host(grid_size * grid_size, 0.0f);
    float max_x = 0.0f, max_y = 0.0f;
    for (const auto& node : instance.nodes) {
        max_x = std::max(max_x, (float)node.x);
        max_y = std::max(max_y, (float)node.y);
    }

    for (const auto& node : instance.nodes) {
        int gx = std::clamp(static_cast<int>((node.x / (max_x + 1e-6)) * (grid_size - 1)), 0, grid_size - 1);
        int gy = std::clamp(static_cast<int>((node.y / (max_y + 1e-6)) * (grid_size - 1)), 0, grid_size - 1);
        density_host[gy * grid_size + gx] += 1.0f;
    }
    CUDA_CHECK(cudaMemcpy(grid.d_density.get(), density_host.data(),
                          grid_size * grid_size * sizeof(float), cudaMemcpyHostToDevice));

    // 2. Solve Poisson EXACTLY ONCE before the loop (resolves potential overwrite issue)
    grid.solve_poisson();

    // 3. Bind potential grid as a texture ONCE before the loop (massive performance improvement)
    cudaArray_t cuArray;
    cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc<float>();
    CUDA_CHECK(cudaMallocArray(&cuArray, &channelDesc, grid_size, grid_size));
    CUDA_CHECK(cudaMemcpy2DToArray(cuArray, 0, 0, grid.d_density.get(),
                                  grid_size * sizeof(float), grid_size * sizeof(float),
                                  grid_size, cudaMemcpyDeviceToDevice));

    cudaResourceDesc resDesc;
    memset(&resDesc, 0, sizeof(resDesc));
    resDesc.resType = cudaResourceTypeArray;
    resDesc.res.array.array = cuArray;

    cudaTextureDesc texDesc;
    memset(&texDesc, 0, sizeof(texDesc));
    texDesc.addressMode[0] = cudaAddressModeClamp;
    texDesc.addressMode[1] = cudaAddressModeClamp;
    texDesc.filterMode = cudaFilterModeLinear;
    texDesc.readMode = cudaReadModeElementType;
    texDesc.normalizedCoords = 1; // Coordinate space mapped to [0.0, 1.0]

    cudaTextureObject_t potentialTex = 0;
    CUDA_CHECK(cudaCreateTextureObject(&potentialTex, &resDesc, &texDesc, NULL));

    // 4. Initialize Ring coordinates as a small circle around normalized centroid
    float centroid_x = 0.5f, centroid_y = 0.5f;
    float radius = 0.1f;
    std::vector<float> host_Y(M * D);
    for (int j = 0; j < M; ++j) {
        float angle = 2.0f * M_PI * j / M;
        host_Y[j * D] = centroid_x + radius * std::cos(angle);
        host_Y[j * D + 1] = centroid_y + radius * std::sin(angle);
    }
    CUDA_CHECK(cudaMemcpy(state.get_Y(), host_Y.data(), M * D * sizeof(float), cudaMemcpyHostToDevice));

    // 5. Optimized High-throughput integration loop
    float dt = 0.1f;
    float mu = 0.9f;
    for (int i = 0; i < config.num_iterations; ++i) {
        compute_elastic_force(state, config.beta, schedule.K);
        launch_advective_force_fft(state.get_Y(), state.get_forces(), potentialTex, M, D, grid_size, config.alpha);
        launch_nesterov_pde(state.get_Y(), state.get_V(), state.get_forces(), M, D, dt, mu);
        schedule.step();
    }

    // 6. Cleanup GPU texture and arrays after loop execution
    CUDA_CHECK(cudaDestroyTextureObject(potentialTex));
    CUDA_CHECK(cudaFreeArray(cuArray));

    // Copy ring positions back and unnormalize coordinates
    CUDA_CHECK(cudaMemcpy(host_Y.data(), state.get_Y(), M * D * sizeof(float), cudaMemcpyDeviceToHost));

    std::vector<float> host_nodes(N * D);
    for (int i = 0; i < N; ++i) {
        host_nodes[i * D] = static_cast<float>(instance.nodes[i].x / (max_x + 1e-6));
        host_nodes[i * D + 1] = static_cast<float>(instance.nodes[i].y / (max_y + 1e-6));
    }

    std::vector<int> bmus(N);
    find_bmus_cuda(host_nodes.data(), N, host_Y.data(), M, D, bmus.data());

    std::vector<int> tour_indices(N);
    std::iota(tour_indices.begin(), tour_indices.end(), 0);
    std::sort(tour_indices.begin(), tour_indices.end(), [&](int a, int b) {
        return bmus[a] < bmus[b];
    });

    Tour tour;
    tour.nodes = std::move(tour_indices);
    deduplicate_and_cleanup(tour.nodes, instance);
    tour.validate(instance);
    return tour;
}

} // namespace approach2
} // namespace tsp
