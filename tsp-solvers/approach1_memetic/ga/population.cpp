#include "chromosome.hpp"
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>

namespace tsp {
namespace approach1 {

class Island {
public:
    std::vector<Chromosome> individuals;
    int id;

    void evolve_step() {
        // Selection, Crossover, Mutation on this island
    }

    Chromosome get_best() {
        return *std::min_element(individuals.begin(), individuals.end(),
            [](const Chromosome& a, const Chromosome& b) { return a.fitness < b.fitness; });
    }
};

class Population {
    std::vector<Island> islands;
    int num_islands;
    int migration_interval = 50;
    int generation_count = 0;

public:
    Population(int n_islands, int pop_per_island) : num_islands(n_islands) {
        for (int i = 0; i < n_islands; ++i) {
            Island island;
            island.id = i;
            // Initialize random chromosomes
            islands.push_back(island);
        }
    }

    void evolve() {
        #pragma omp parallel for
        for (int i = 0; i < num_islands; ++i) {
            islands[i].evolve_step();
        }

        generation_count++;
        if (generation_count % migration_interval == 0) {
            migrate();
        }
    }

    void migrate() {
        // Ring topology migration
        std::cout << "Migration between islands starting..." << std::endl;
        for (int i = 0; i < num_islands; ++i) {
            int next = (i + 1) % num_islands;
            Chromosome elite = islands[i].get_best();
            // Replace worst in next island
            auto& next_pop = islands[next].individuals;
            auto worst_it = std::max_element(next_pop.begin(), next_pop.end(),
                [](const Chromosome& a, const Chromosome& b) { return a.fitness < b.fitness; });
            if (worst_it != next_pop.end()) {
                *worst_it = elite;
            }
        }
    }
};

} // namespace approach1
} // namespace tsp
