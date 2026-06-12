#include <vector>
#include <algorithm>

namespace tsp {
namespace approach2 {

// Simple BMU search for now. Real production would use a proper KD-tree or FAISS.
void find_bmus(const float* nodes, int N, const float* ring, int M, int D, int* bmus) {
    for (int i = 0; i < N; ++i) {
        float min_dist = 1e18;
        int best_j = 0;
        for (int j = 0; j < M; ++j) {
            float dist = 0;
            for (int d = 0; d < D; ++d) {
                float diff = nodes[i * D + d] - ring[j * D + d];
                dist += diff * diff;
            }
            if (dist < min_dist) {
                min_dist = dist;
                best_j = j;
            }
        }
        bmus[i] = best_j;
    }
}

} // namespace approach2
} // namespace tsp
