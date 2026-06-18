#include "chromosome.hpp"
#include <random>
#include <algorithm>

namespace tsp {
namespace approach1 {

void levy_flight_mutation(Chromosome& c) {
    // Simple baseline fallback: standard swap mutation
    if (c.genes.size() > 1 && (rand() % 100 < 5)) { // 5% mutation rate
        int i = rand() % c.genes.size();
        int j = rand() % c.genes.size();
        std::swap(c.genes[i], c.genes[j]);
    }

    // TODO: Implement Levy-Flight stochastic mutation operator
}

} // namespace approach1
} // namespace tsp
