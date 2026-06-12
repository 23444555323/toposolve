#ifndef TSP_SOLVERS_INTERFACE_HPP
#define TSP_SOLVERS_INTERFACE_HPP

#include "graph.hpp"
#include "solution.hpp"
#include "types.hpp"

namespace tsp {

class TSPSolver {
protected:
    Graph graph;
public:
    virtual void load_graph(const Graph& g) {
        graph = g;
    }
    virtual Solution solve() = 0;
    virtual BenchmarkResult benchmark(int runs);
    virtual ~TSPSolver() = default;
};

// Defined in benchmark.cpp
BenchmarkResult run_benchmark(TSPSolver* solver, int runs);

inline BenchmarkResult TSPSolver::benchmark(int runs) {
    return run_benchmark(this, runs);
}

} // namespace tsp

#endif // TSP_SOLVERS_INTERFACE_HPP
