import matplotlib.pyplot as plt
import numpy as np

def visualize_tsp(nodes, ring, tour=None, title="TSP Solver"):
    plt.figure(figsize=(8, 8))
    plt.scatter(nodes[:, 0], nodes[:, 1], c='red', label='Cities')

    if ring is not None:
        ring_closed = np.vstack([ring, ring[0]])
        plt.plot(ring_closed[:, 0], ring_closed[:, 1], 'b--', alpha=0.5, label='Elastic Ring')

    if tour is not None:
        tour_nodes = nodes[tour]
        tour_closed = np.vstack([tour_nodes, tour_nodes[0]])
        plt.plot(tour_closed[:, 0], tour_closed[:, 1], 'g-', linewidth=2, label='Final Tour')

    plt.title(title)
    plt.legend()
    plt.show()
