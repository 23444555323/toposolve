import numpy as np
from approach1_memetic import MemeticSolver
from approach2_elastic import ElasticRingSolver

def generate_problem(N=20):
    return np.random.rand(N, 2)

def check_parity():
    nodes = generate_problem(50)

    # Solve with Approach 1
    solver1 = MemeticSolver(nodes)
    tour1 = solver1.solve()
    len1 = np.sum(np.sqrt(np.sum((nodes[tour1] - nodes[np.roll(tour1, -1)])**2, axis=1)))

    # Solve with Approach 2
    solver2 = ElasticRingSolver(nodes)
    solver2.anneal(epochs=500)
    tour2 = solver2.get_tour()
    len2 = np.sum(np.sqrt(np.sum((nodes[tour2] - nodes[np.roll(tour2, -1)])**2, axis=1)))

    print(f"Approach 1 Length: {len1:.4f}")
    print(f"Approach 2 Length: {len2:.4f}")
    print(f"Difference: {abs(len1 - len2) / max(len1, len2) * 100:.2f}%")

if __name__ == "__main__":
    check_parity()
