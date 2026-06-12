#ifndef TSP_SOLVERS_SOLUTION_HPP
#define TSP_SOLVERS_SOLUTION_HPP

#include <vector>
#include <numeric>
#include <algorithm>
#include <set>
#include "graph.hpp"

namespace tsp {

struct Solution {
    std::vector<int> tour;
    double tour_length = 0.0;
    bool is_valid = false;
    double solve_time_ms = 0.0;

    void validate(const Graph& g) {
        if (tour.size() != static_cast<size_t>(g.size())) {
            is_valid = false;
            return;
        }

        std::set<int> visited(tour.begin(), tour.end());
        if (visited.size() != static_cast<size_t>(g.size())) {
            is_valid = false;
            return;
        }

        tour_length = 0.0;
        for (size_t i = 0; i < tour.size(); ++i) {
            int u = tour[i];
            int v = tour[(i + 1) % tour.size()];
            tour_length += g.distance(u, v);
        }
        is_valid = true;
    }
};

} // namespace tsp

#endif // TSP_SOLVERS_SOLUTION_HPP
