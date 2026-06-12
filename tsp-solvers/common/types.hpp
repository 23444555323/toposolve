#ifndef TSP_SOLVERS_TYPES_HPP
#define TSP_SOLVERS_TYPES_HPP

#include <vector>
#include <string>
#include "graph.hpp"

namespace tsp {

using ProblemInstance = Graph;

struct SolverConfig {
    // Shared configurations
    int num_iterations = 1000;
    double convergence_threshold = 1e-6;

    // Approach 1 specific
    int population_size = 100;
    double mutation_rate = 0.05;

    // Approach 2 specific
    double alpha = 1.0;  // Advective weight
    double beta = 1.0;   // Elastic weight
    double beta_b_max = 1.0; // Barrier max weight
    double gamma = 1.5;  // M = gamma * N
    double K0 = 1.0;     // Initial temperature
    double epsilon = 0.03; // Temperature decay
    double epsilon_soft = 1e-6; // Barrier softening constant
};

struct BenchmarkResult {
    double min_length;
    double max_length;
    double avg_length;
    double avg_time_ms;
    int num_runs;
};

} // namespace tsp

#endif // TSP_SOLVERS_TYPES_HPP
