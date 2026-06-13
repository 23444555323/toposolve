#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <set>
#include <cuda_runtime.h>
#include "approach2_elastic_ring/vse/ring_state.hpp"
#include "approach2_elastic_ring/fft_solver/fft_grid.hpp"
#include "approach2_elastic_ring/engine.hpp"
#include "common/graph.hpp"
#include "common/solution.hpp"

// External declarations for kernels/functions under test
namespace tsp {
namespace approach2 {
void launch_advective_force_fft(float* Y, float* forces, cudaTextureObject_t potentialTex, int M, int D, int grid_size, float alpha);
void deduplicate_and_cleanup(std::vector<int>& tour, const Graph& g);
}
}

using namespace tsp;
using namespace tsp::approach2;

void test_1_poisson_solver() {
    std::cout << "[Test 1] Poisson Solver Validation... " << std::flush;
    int size = 128;
    FFTGrid grid(size);

    std::vector<float> host_density(size * size, 0.0f);
    host_density[(size/2) * size + (size/2)] = 100.0f; // Center spike

    CUDA_CHECK(cudaMemcpy(grid.d_density, host_density.data(), size * size * sizeof(float), cudaMemcpyHostToDevice));
    grid.solve_poisson();

    std::vector<float> host_potential(size * size);
    CUDA_CHECK(cudaMemcpy(host_potential.data(), grid.d_density, size * size * sizeof(float), cudaMemcpyDeviceToHost));

    bool non_zero = false;
    for(float v : host_potential) if(std::abs(v) > 1e-6f) non_zero = true;
    assert(non_zero && "Potential field is zero!");

    // Check decay (crude)
    float center_v = host_potential[(size/2) * size + (size/2)];
    float edge_v = host_potential[0];
    assert(std::abs(center_v) > std::abs(edge_v) && "Potential does not decay away from spike!");

    std::cout << "PASSED" << std::endl;
}

void test_2_advective_force() {
    std::cout << "[Test 2] Advective Force (Texture Object)... " << std::flush;
    int size = 64;
    int M = 1;
    int D = 2;

    RingState state(M, D);

    cudaArray_t cuArray;
    cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc<float>();
    CUDA_CHECK(cudaMallocArray(&cuArray, &channelDesc, size, size));

    std::vector<float> host_potential(size * size, 0.0f);
    host_potential[32 * size + 32] = 10.0f;
    host_potential[32 * size + 33] = 8.0f;
    CUDA_CHECK(cudaMemcpy2DToArray(cuArray, 0, 0, host_potential.data(), size * sizeof(float), size * sizeof(float), size, cudaMemcpyHostToDevice));

    cudaResourceDesc resDesc;
    memset(&resDesc, 0, sizeof(resDesc));
    resDesc.resType = cudaResourceTypeArray;
    resDesc.res.array.array = cuArray;

    cudaTextureDesc texDesc;
    memset(&texDesc, 0, sizeof(texDesc));
    texDesc.addressMode[0] = cudaAddressModeWrap;
    texDesc.addressMode[1] = cudaAddressModeWrap;
    texDesc.filterMode = cudaFilterModeLinear;
    texDesc.readMode = cudaReadModeElementType;
    texDesc.normalizedCoords = 0;

    cudaTextureObject_t texObj = 0;
    CUDA_CHECK(cudaCreateTextureObject(&texObj, &resDesc, &texDesc, NULL));

    float host_Y[2] = {31.5f, 32.0f};
    CUDA_CHECK(cudaMemcpy(state.get_Y(), host_Y, 2 * sizeof(float), cudaMemcpyHostToDevice));

    launch_advective_force_fft(state.get_Y(), state.get_forces(), texObj, M, D, size, 1.0f);

    float host_F[2];
    CUDA_CHECK(cudaMemcpy(host_F, state.get_forces(), 2 * sizeof(float), cudaMemcpyDeviceToHost));

    assert(host_F[0] > 0 && "Force not pointing toward peak in X!");

    CUDA_CHECK(cudaDestroyTextureObject(texObj));
    CUDA_CHECK(cudaFreeArray(cuArray));
    std::cout << "PASSED" << std::endl;
}

void test_3_resource_safety() {
    std::cout << "[Test 3] RingState RAII Move Semantics... " << std::flush;
    {
        RingState s1(10, 2);
        float* ptr = s1.get_Y();
        RingState s2 = std::move(s1);
        assert(s1.get_Y() == nullptr);
        assert(s2.get_Y() == ptr);
    }
    std::cout << "PASSED" << std::endl;
}

void test_4_integration() {
    std::cout << "[Test 4] End-to-End Pipeline Wiring... " << std::flush;
    Graph g;
    for(int i=0; i<10; ++i) g.add_node(rand()%100, rand()%100);

    ElasticEngine engine;
    SolverConfig config;
    config.num_iterations = 10;

    Tour tour = engine.solve(g, config);
    assert(!tour.nodes.empty());
    std::cout << "PASSED" << std::endl;
}

void test_5_dtml_integrity() {
    std::cout << "[Test 5] DTML Deduplication & Hamiltonian Validation... " << std::flush;
    Graph g;
    int N = 10;
    for(int i=0; i<N; ++i) g.add_node(i, i);

    std::vector<int> raw_tour = {0, 0, 1, 2, 2, 3, 4, 5, 6, 7, 8, 9}; // Duplicates
    deduplicate_and_cleanup(raw_tour, g);

    Tour tour;
    tour.nodes = raw_tour;
    tour.validate(g);

    assert(tour.is_valid && "Tour is not a valid Hamiltonian cycle!");
    assert(tour.nodes.size() == (size_t)N);
    std::set<int> unique_nodes(tour.nodes.begin(), tour.nodes.end());
    assert(unique_nodes.size() == (size_t)N);

    std::cout << "PASSED" << std::endl;
}

int main() {
    try {
        test_1_poisson_solver();
        test_2_advective_force();
        test_3_resource_safety();
        test_4_integration();
        test_5_dtml_integrity();
        std::cout << "\nAll verification tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "\nTest failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
