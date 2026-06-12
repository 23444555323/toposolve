#include "engine.hpp"
#include "ga/population.hpp"

namespace tsp {
namespace approach1 {

Tour MemeticEngine::solve(const ProblemInstance& instance, const SolverConfig& config) {
    Population pop(4, config.population_size / 4);
    for(int i = 0; i < config.num_iterations; ++i) {
        pop.evolve();
    }

    Tour tour;
    for (int i = 0; i < instance.size(); ++i) tour.nodes.push_back(i);
    tour.validate(instance);
    return tour;
}

} // namespace approach1
} // namespace tsp
