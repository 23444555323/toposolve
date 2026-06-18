#ifndef TSP_SOLVERS_DTML_HPP
#define TSP_SOLVERS_DTML_HPP

#include <vector>
#include "common/graph.hpp"

namespace tsp {
namespace approach2 {

void deduplicate_and_cleanup(std::vector<int>& tour, const Graph& g);
void find_bmus(const float* nodes, int N, const float* ring, int M, int D, int* bmus);
void find_bmus_cuda(const float* nodes, int N, const float* ring, int M, int D, int* bmus);

} // namespace approach2
} // namespace tsp

#endif // TSP_SOLVERS_DTML_HPP
