#include "engine.hpp"
#include "ga/population.hpp"

namespace tsp {
namespace approach1 {

Tour MemeticEngine::solve(const ProblemInstance& instance, const SolverConfig& config) {
    Population pop(4, config.population_size / 4);
    for(int i = 0; i < config.num_iterations; ++i) {
        pop.evolve();
    }

    // Extract best chromosome from all islands
    // For now, take from the first island as a simplified integration
    Chromosome best = pop.get_best_global();

    Tour tour;
    if (best.genes.empty()) {
        // Fallback to sequential if no evolution happened
        for (int i = 0; i < instance.size(); ++i) tour.nodes.push_back(i);
    } else {
        tour.nodes = best.genes;
    }

    tour.validate(instance);
    return tour;
}

} // namespace approach1
} // namespace tsp
