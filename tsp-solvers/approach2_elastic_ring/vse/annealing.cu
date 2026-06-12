namespace tsp {
namespace approach2 {

struct AnnealingSchedule {
    float K;
    float epsilon;

    void step() {
        K *= (1.0f - epsilon);
    }
};

} // namespace approach2
} // namespace tsp
