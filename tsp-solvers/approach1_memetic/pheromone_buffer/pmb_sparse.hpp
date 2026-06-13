#ifndef TSP_SOLVERS_PMB_SPARSE_HPP
#define TSP_SOLVERS_PMB_SPARSE_HPP

#include "pmb.hpp"
#include <unordered_map>
#include <utility>

namespace tsp {
namespace approach1 {

struct PairHash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

class SparsePheromoneBuffer : public PheromoneBuffer {
    std::unordered_map<std::pair<int, int>, float, PairHash> data;
public:
    void update(int i, int j, float val) override;
    float get(int i, int j) override;
    void decay(float rate) override;
};

} // namespace approach1
} // namespace tsp

#endif // TSP_SOLVERS_PMB_SPARSE_HPP
