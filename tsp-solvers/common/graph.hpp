#ifndef TSP_SOLVERS_GRAPH_HPP
#define TSP_SOLVERS_GRAPH_HPP

#include <vector>
#include <cmath>
#include <stdexcept>
#include "types.hpp"

namespace tsp {

struct Node {
    int id;
    double x, y;
    std::vector<double> embedding; // For Approach 2 GNN output
};

class Graph {
public:
    std::vector<Node> nodes;

    void add_node(double x, double y) {
        nodes.push_back({static_cast<int>(nodes.size()), x, y, {}});
    }

    double distance(int i, int j, DistanceMetric metric = DistanceMetric::EUCLIDEAN) const {
        double dx = std::abs(nodes[i].x - nodes[j].x);
        double dy = std::abs(nodes[i].y - nodes[j].y);
        switch (metric) {
            case DistanceMetric::MANHATTAN: return dx + dy;
            case DistanceMetric::CHEBYSHEV: return std::max(dx, dy);
            case DistanceMetric::EUCLIDEAN:
            default: return std::sqrt(dx*dx + dy*dy);
        }
    }

    int size() const {
        return static_cast<int>(nodes.size());
    }
};

} // namespace tsp

#endif // TSP_SOLVERS_GRAPH_HPP
