#ifndef TSP_SOLVERS_SOLUTION_HPP
#define TSP_SOLVERS_SOLUTION_HPP

#include <vector>
#include <numeric>
#include <algorithm>
#include <set>
#include "graph.hpp"

namespace tsp {

struct Tour {
    std::vector<int> nodes;
    double length = 0.0;
    bool is_valid = false;
    double solve_time_ms = 0.0;

    void validate(const Graph& g) {
        if (nodes.size() != static_cast<size_t>(g.size())) {
            is_valid = false;
            return;
        }

        std::set<int> visited(nodes.begin(), nodes.end());
        if (visited.size() != static_cast<size_t>(g.size())) {
            is_valid = false;
            return;
        }

        length = 0.0;
        for (size_t i = 0; i < nodes.size(); ++i) {
            int u = nodes[i];
            int v = nodes[(i + 1) % nodes.size()];
            length += g.distance(u, v);
        }
        is_valid = true;
    }
};

using Solution = Tour; // Alias for backward compatibility if needed

} // namespace tsp

#endif // TSP_SOLVERS_SOLUTION_HPP
