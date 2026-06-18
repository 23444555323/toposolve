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
void find_bmus(const float* nodes, int N, const float* ring, int M, int D, int* bmus);
void deduplicate_and_cleanup(std::vector<int>& tour, const Graph& g);

Tour ElasticEngine::solve(const ProblemInstance& instance, const SolverConfig& config) {
    int N = instance.size();
    int M = static_cast<int>(config.gamma * N);
    int D = 2;

    RingState state(M, D);
    FFTGrid grid(256);
    AnnealingSchedule schedule { static_cast<float>(config.K0), static_cast<float>(config.epsilon) };

    // 1. Initialize Ring coordinates as a small circle around centroid
    float centroid_x = 0.0f, centroid_y = 0.0f;
    for (const auto& node : instance.nodes) {
        centroid_x += static_cast<float>(node.x);
        centroid_y += static_cast<float>(node.y);
    }
    centroid_x /= N;
    centroid_y /= N;

    float radius = 0.1f;
    std::vector<float> host_Y(M * D);
    for (int j = 0; j < M; ++j) {
        float angle = 2.0f * M_PI * j / M;
        host_Y[j * D] = centroid_x + radius * std::cos(angle);
        host_Y[j * D + 1] = centroid_y + radius * std::sin(angle);
    }
    CUDA_CHECK(cudaMemcpy(state.get_Y(), host_Y.data(), M * D * sizeof(float), cudaMemcpyHostToDevice));

    // 2. Integration loop (Euler-Nesterov step)
    float dt = 0.1f;
    float mu = 0.9f;
    for (int i = 0; i < config.num_iterations; ++i) {
        grid.solve_poisson();
        compute_elastic_force(state, config.beta, schedule.K);

        // Potential texture setup and advective force evaluation omitted for now

        launch_nesterov_pde(state.get_Y(), state.get_V(), state.get_forces(), M, D, dt, mu);
        schedule.step();
    }

    // Copy ring positions back to host
    CUDA_CHECK(cudaMemcpy(host_Y.data(), state.get_Y(), M * D * sizeof(float), cudaMemcpyDeviceToHost));

    // Prepare host city positions
    std::vector<float> host_nodes(N * D);
    for (int i = 0; i < N; ++i) {
        host_nodes[i * D] = static_cast<float>(instance.nodes[i].x);
        host_nodes[i * D + 1] = static_cast<float>(instance.nodes[i].y);
    }

    // 3. BMU (Best Matching Unit) Search
    std::vector<int> bmus(N);
    find_bmus(host_nodes.data(), N, host_Y.data(), M, D, bmus.data());

    // 4. Construct the Tour based on sorted BMU index positions along the elastic ring
    std::vector<int> tour_indices(N);
    std::iota(tour_indices.begin(), tour_indices.end(), 0);
    std::sort(tour_indices.begin(), tour_indices.end(), [&](int a, int b) {
        return bmus[a] < bmus[b];
    });

    Tour tour;
    tour.nodes = std::move(tour_indices);

    // 5. Run the 2-opt cleanup sweep
    deduplicate_and_cleanup(tour.nodes, instance);
    tour.validate(instance);
    return tour;
}

} // namespace approach2
} // namespace tsp
