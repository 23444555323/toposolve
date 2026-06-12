#ifndef TSP_SOLVERS_MEMETIC_ENGINE_HPP
#define TSP_SOLVERS_MEMETIC_ENGINE_HPP

#include "../../common/abstract_solver.hpp"

namespace tsp {
namespace approach1 {

class MemeticEngine : public TSPSolver {
public:
    Tour solve(const ProblemInstance& instance, const SolverConfig& config) override;
};

} // namespace approach1
} // namespace tsp

#endif // TSP_SOLVERS_MEMETIC_ENGINE_HPP
