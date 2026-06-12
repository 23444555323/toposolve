#ifndef TSP_SOLVERS_PMB_HPP
#define TSP_SOLVERS_PMB_HPP

namespace tsp {
namespace approach1 {

class PheromoneBuffer {
public:
    virtual void update(int i, int j, float val) = 0;
    virtual float get(int i, int j) = 0;
    virtual void decay(float rate) = 0;
    virtual ~PheromoneBuffer() = default;
};

} // namespace approach1
} // namespace tsp

#endif // TSP_SOLVERS_PMB_HPP
