#include "population.hpp"
#include <algorithm>
#include <iostream>
#include <omp.h>
#include <random>

namespace tsp {
namespace approach1 {

void Island::evolve_step() {
    // Evaluate fitness of chromosomes (mock/basic evolution logic placeholder)
    for (auto& ind : individuals) {
        if (!ind.genes.empty()) {
            ind.fitness = 0.0;
        }
    }
}

Chromosome Island::get_best() {
    if (individuals.empty()) {
        return Chromosome{{}, 1e18}; // Safe default to prevent dereferencing empty vector
    }
    return *std::min_element(individuals.begin(), individuals.end(),
        [](const Chromosome& a, const Chromosome& b) { return a.fitness < b.fitness; });
}

Population::Population(int n_islands, int pop_per_island) : num_islands(n_islands) {
    for (int i = 0; i < n_islands; ++i) {
        Island island;
        island.id = i;
        // Allocate and initialize population chromosomes to avoid segfaults
        for (int j = 0; j < pop_per_island; ++j) {
            Chromosome c;
            // Initialize with a random permutation for validity
            // Mocking for now, in real scenario we'd use instance size
            c.fitness = static_cast<double>(rand()) / RAND_MAX;
            island.individuals.push_back(c);
        }
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

Chromosome Population::get_best_global() {
    Chromosome best = islands[0].get_best();
    for (int i = 1; i < num_islands; ++i) {
        Chromosome current = islands[i].get_best();
        if (current.fitness < best.fitness) {
            best = current;
        }
    }
    return best;
}

} // namespace approach1
} // namespace tsp
