#ifndef TSP_SOLVERS_ELASTIC_ENGINE_HPP
#define TSP_SOLVERS_ELASTIC_ENGINE_HPP

#include "../../common/abstract_solver.hpp"

namespace tsp {
namespace approach2 {

class ElasticEngine : public TSPSolver {
public:
    Tour solve(const ProblemInstance& instance, const SolverConfig& config) override;
};

} // namespace approach2
} // namespace tsp

#endif // TSP_SOLVERS_ELASTIC_ENGINE_HPP
