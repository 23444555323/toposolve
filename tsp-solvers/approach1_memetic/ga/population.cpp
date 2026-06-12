#include "population.hpp"
#include <algorithm>
#include <iostream>
#include <omp.h>

namespace tsp {
namespace approach1 {

void Island::evolve_step() {}

Chromosome Island::get_best() {
    return *std::min_element(individuals.begin(), individuals.end(),
        [](const Chromosome& a, const Chromosome& b) { return a.fitness < b.fitness; });
}

Population::Population(int n_islands, int pop_per_island) : num_islands(n_islands) {
    for (int i = 0; i < n_islands; ++i) {
        Island island;
        island.id = i;
        islands.push_back(island);
    }
}

void Population::evolve() {
    #pragma omp parallel for
    for (int i = 0; i < num_islands; ++i) {
        islands[i].evolve_step();
    }
    generation_count++;
    if (generation_count % migration_interval == 0) migrate();
}

void Population::migrate() {
    for (int i = 0; i < num_islands; ++i) {
        int next = (i + 1) % num_islands;
        Chromosome elite = islands[i].get_best();
        auto& next_pop = islands[next].individuals;
        if (!next_pop.empty()) {
            auto worst_it = std::max_element(next_pop.begin(), next_pop.end(),
                [](const Chromosome& a, const Chromosome& b) { return a.fitness < b.fitness; });
            *worst_it = elite;
        }
    }
}

} // namespace approach1
} // namespace tsp
