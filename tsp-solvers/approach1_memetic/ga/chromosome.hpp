#ifndef TSP_SOLVERS_CHROMOSOME_HPP
#define TSP_SOLVERS_CHROMOSOME_HPP

#include <vector>

namespace tsp {
namespace approach1 {

struct Chromosome {
    std::vector<int> genes;
    double fitness;
};

} // namespace approach1
} // namespace tsp

#endif // TSP_SOLVERS_CHROMOSOME_HPP
