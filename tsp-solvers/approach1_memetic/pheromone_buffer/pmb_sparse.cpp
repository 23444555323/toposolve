#include "pmb.hpp"
#include <map>

namespace tsp {
namespace approach1 {

class SparsePheromoneBuffer : public PheromoneBuffer {
    std::map<std::pair<int, int>, float> data;
public:
    void update(int i, int j, float val) override {
        data[{i, j}] += val;
    }
    float get(int i, int j) override {
        auto it = data.find({i, j});
        return (it != data.end()) ? it->second : 0.01f;
    }
    void decay(float rate) override {
        for (auto& pair : data) pair.second *= rate;
    }
};

} // namespace approach1
} // namespace tsp
