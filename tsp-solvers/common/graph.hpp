#ifndef TSP_SOLVERS_GRAPH_HPP
#define TSP_SOLVERS_GRAPH_HPP

#include <vector>
#include <cmath>
#include <stdexcept>

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

    double distance(int i, int j) const {
        double dx = nodes[i].x - nodes[j].x;
        double dy = nodes[i].y - nodes[j].y;
        return std::sqrt(dx*dx + dy*dy);
    }

    int size() const {
        return static_cast<int>(nodes.size());
    }
};

} // namespace tsp

#endif // TSP_SOLVERS_GRAPH_HPP
