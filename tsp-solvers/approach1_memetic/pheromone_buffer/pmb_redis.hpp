#include "pmb.hpp"
#include <string>

namespace tsp {
namespace approach1 {

class RedisPheromoneBuffer : public PheromoneBuffer {
    std::string redis_host;
    int redis_port;
    std::string key_prefix;

public:
    RedisPheromoneBuffer(const std::string& host = "localhost", int port = 6379, const std::string& prefix = "tsp_pheromone");
    void update(int i, int j, float val) override;
    float get(int i, int j) override;
    void decay(float rate) override;
};

} // namespace approach1
} // namespace tsp
