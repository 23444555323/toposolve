#include "population.hpp"
#include <algorithm>
#include <iostream>
#include <omp.h>
#include <random>
#include <numeric>

namespace tsp {
namespace approach1 {

void Island::evolve_step(const Graph& instance) {
    // Calculate actual route distance for each chromosome
    for (auto& ind : individuals) {
        if (!ind.genes.empty()) {
            double route_len = 0.0;
            for (size_t i = 0; i < ind.genes.size(); ++i) {
                int u = ind.genes[i];
                int v = ind.genes[(i + 1) % ind.genes.size()];
                route_len += instance.distance(u, v);
            }
            ind.fitness = route_len;
        }
    }
}

Chromosome Island::get_best() {
    if (individuals.empty()) {
        return Chromosome{{}, 1e18};
    }
    return *std::min_element(individuals.begin(), individuals.end(),
        [](const Chromosome& a, const Chromosome& b) { return a.fitness < b.fitness; });
}

Population::Population(int n_islands, int pop_per_island, int num_nodes) : num_islands(n_islands) {
    std::random_device rd;
    std::mt19937 rng(rd());

    for (int i = 0; i < n_islands; ++i) {
        Island island;
        island.id = i;
        for (int j = 0; j < pop_per_island; ++j) {
            Chromosome c;
            c.genes.resize(num_nodes);
            std::iota(c.genes.begin(), c.genes.end(), 0);
            std::shuffle(c.genes.begin(), c.genes.end(), rng);
            c.fitness = 1e18;
            island.individuals.push_back(c);
        }
        islands.push_back(island);
    }
}

void Population::evolve(const Graph& instance) {
    #pragma omp parallel for
    for (int i = 0; i < num_islands; ++i) {
        islands[i].evolve_step(instance);
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
