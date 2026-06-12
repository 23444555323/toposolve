#include "common/solver_interface.hpp"
#include "approach1_memetic/engine.hpp"
#include "approach2_elastic_ring/engine.hpp"
#include <iostream>

int main() {
    tsp::Graph g;
    g.add_node(0, 0);
    g.add_node(1, 0);
    g.add_node(1, 1);
    g.add_node(0, 1);

    tsp::approach1::MemeticEngine engine1;
    engine1.load_graph(g);
    auto sol1 = engine1.solve();
    std::cout << "Engine 1 Tour Length: " << sol1.tour_length << std::endl;

    tsp::approach2::ElasticEngine engine2;
    engine2.load_graph(g);
    auto sol2 = engine2.solve();
    std::cout << "Engine 2 Tour Length: " << sol2.tour_length << std::endl;

    return 0;
}
