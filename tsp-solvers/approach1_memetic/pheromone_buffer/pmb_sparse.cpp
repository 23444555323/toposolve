#include "pmb_sparse.hpp"

namespace tsp {
namespace approach1 {

void SparsePheromoneBuffer::update(int i, int j, float val) {
    data[{i, j}] += val;
}

float SparsePheromoneBuffer::get(int i, int j) {
    auto it = data.find({i, j});
    return (it != data.end()) ? it->second : 0.01f;
}

void SparsePheromoneBuffer::decay(float rate) {
    for (auto& pair : data) pair.second *= rate;
}

} // namespace approach1
} // namespace tsp
