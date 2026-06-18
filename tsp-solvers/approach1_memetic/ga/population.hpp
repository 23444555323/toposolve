#ifndef TSP_SOLVERS_POPULATION_HPP
#define TSP_SOLVERS_POPULATION_HPP

#include "chromosome.hpp"
#include "../../common/graph.hpp"
#include <vector>

namespace tsp {
namespace approach1 {

class Island {
public:
    std::vector<Chromosome> individuals;
    int id;
    void evolve_step(const Graph& instance);
    Chromosome get_best();
};

class Population {
    std::vector<Island> islands;
    int num_islands;
    int migration_interval = 50;
    int generation_count = 0;

public:
    Population(int n_islands, int pop_per_island, int num_nodes);
    void evolve(const Graph& instance);
    void migrate();
    Chromosome get_best_global();
};

} // namespace approach1
} // namespace tsp

#endif // TSP_SOLVERS_POPULATION_HPP
