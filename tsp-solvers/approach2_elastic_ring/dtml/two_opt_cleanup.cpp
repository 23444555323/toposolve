#include "dtml.hpp"
#include <algorithm>
#include <set>

namespace tsp {
namespace approach2 {

void deduplicate_and_cleanup(std::vector<int>& tour, const Graph& g) {
    std::set<int> seen;
    std::vector<int> unique_tour;
    for (int node : tour) {
        if (seen.find(node) == seen.end()) {
            unique_tour.push_back(node);
            seen.insert(node);
        }
    }

    if (unique_tour.size() < static_cast<size_t>(g.size())) {
        for (int i = 0; i < g.size(); ++i) {
            if (seen.find(i) == seen.end()) {
                unique_tour.push_back(i);
            }
        }
    }
    tour = std::move(unique_tour);

    bool improved = true;
    while (improved) {
        improved = false;
        for (size_t i = 1; i < tour.size() - 2; ++i) {
            for (size_t j = i + 1; j < tour.size(); ++j) {
                int u1 = tour[i-1];
                int v1 = tour[i];
                int u2 = tour[j-1];
                int v2 = tour[j % tour.size()];

                double d_old = g.distance(u1, v1) + g.distance(u2, v2);
                double d_new = g.distance(u1, u2) + g.distance(v1, v2);

                if (d_new < d_old - 1e-6) {
                    std::reverse(tour.begin() + i, tour.begin() + j);
                    improved = true;
                }
            }
        }
    }
}

} // namespace approach2
} // namespace tsp
