#include "common/abstract_solver.hpp"
#include "approach1_memetic/engine.hpp"
#include "approach2_elastic_ring/engine.hpp"
#include <iostream>

int main() {
    tsp::Graph g;
    g.add_node(0, 0);
    g.add_node(1, 0);
    g.add_node(1, 1);
    g.add_node(0, 1);

    tsp::SolverConfig config;

    tsp::approach1::MemeticEngine engine1;
    auto sol1 = engine1.solve(g, config);
    std::cout << "Engine 1 Tour Length: " << sol1.length << std::endl;

    tsp::approach2::ElasticEngine engine2;
    auto sol2 = engine2.solve(g, config);
    std::cout << "Engine 2 Tour Length: " << sol2.length << std::endl;

    return 0;
}
