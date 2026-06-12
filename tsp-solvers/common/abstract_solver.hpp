#ifndef TSP_SOLVERS_ABSTRACT_SOLVER_HPP
#define TSP_SOLVERS_ABSTRACT_SOLVER_HPP

#include "types.hpp"
#include "solution.hpp"

namespace tsp {

class TSPSolver {
public:
    virtual ~TSPSolver() = default;
    virtual Tour solve(const ProblemInstance& instance, const SolverConfig& config) = 0;
};

} // namespace tsp

#endif // TSP_SOLVERS_ABSTRACT_SOLVER_HPP
