#include "engine.hpp"
#include "ga/population.hpp"

namespace tsp {
namespace approach1 {

Tour MemeticEngine::solve(const ProblemInstance& instance, const SolverConfig& config) {
    int N = instance.size();

    Population pop(4, config.population_size / 4, N);

    for(int i = 0; i < config.num_iterations; ++i) {
        pop.evolve(instance);
    }

    Chromosome best = pop.get_best_global();

    Tour tour;
    if (best.genes.empty()) {
        for (int i = 0; i < instance.size(); ++i) tour.nodes.push_back(i);
    } else {
        tour.nodes = best.genes;
    }

    tour.validate(instance);
    return tour;
}

} // namespace approach1
} // namespace tsp
