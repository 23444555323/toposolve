#ifndef TSP_SOLVERS_DTML_HPP
#define TSP_SOLVERS_DTML_HPP

#include <vector>
#include "common/graph.hpp"

namespace tsp {
namespace approach2 {

void deduplicate_and_cleanup(std::vector<int>& tour, const Graph& g);

} // namespace approach2
} // namespace tsp

#endif // TSP_SOLVERS_DTML_HPP
