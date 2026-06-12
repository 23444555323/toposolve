#include "abstract_solver.hpp"
#include <chrono>
#include <iostream>
#include <numeric>
#include <algorithm>

namespace tsp {

BenchmarkResult run_benchmark(TSPSolver* solver, const ProblemInstance& instance, const SolverConfig& config, int runs) {
    std::vector<double> lengths;
    std::vector<double> times;

    for (int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        Tour tour = solver->solve(instance, config);
        auto end = std::chrono::high_resolution_clock::now();

        if (tour.is_valid) {
            lengths.push_back(tour.length);
            times.push_back(std::chrono::duration<double, std::milli>(end - start).count());
        }
    }

    if (lengths.empty()) return {0, 0, 0, 0, 0};

    double min_len = *std::min_element(lengths.begin(), lengths.end());
    double max_len = *std::max_element(lengths.begin(), lengths.end());
    double avg_len = std::accumulate(lengths.begin(), lengths.end(), 0.0) / lengths.size();
    double avg_time = std::accumulate(times.begin(), times.end(), 0.0) / times.size();

    return {min_len, max_len, avg_len, avg_time, static_cast<int>(lengths.size())};
}

} // namespace tsp
