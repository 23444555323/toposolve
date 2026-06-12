#include "chromosome.hpp"
#include <omp.h>

namespace tsp {
namespace approach1 {

void edge_ordered_crossover(const Chromosome& p1, const Chromosome& p2, Chromosome& child) {
    // OpenMP parallelized OX crossover
    #pragma omp parallel
    {
        // crossover logic
    }
}

} // namespace approach1
} // namespace tsp
