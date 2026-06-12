import numpy as np

class MemeticSolver:
    def __init__(self, nodes, pop_size=50):
        self.nodes = nodes
        self.N = len(nodes)
        self.pop_size = pop_size
        self.dist_matrix = self._compute_dist_matrix()
        self.population = [np.random.permutation(self.N) for _ in range(pop_size)]
        self.pheromone = np.ones((self.N, self.N)) / self.N

    def _compute_dist_matrix(self):
        diff = self.nodes[:, np.newaxis, :] - self.nodes[np.newaxis, :, :]
        return np.sqrt(np.sum(diff**2, axis=2))

    def _tour_length(self, tour):
        return np.sum(self.dist_matrix[tour, np.roll(tour, -1)])

    def local_search_2opt(self, tour):
        best_tour = tour.copy()
        best_len = self._tour_length(best_tour)
        improved = True
        while improved:
            improved = False
            for i in range(1, self.N - 2):
                for j in range(i + 1, self.N):
                    if j - i == 1: continue
                    new_tour = best_tour.copy()
                    new_tour[i:j] = best_tour[i:j][::-1]
                    new_len = self._tour_length(new_tour)
                    if new_len < best_len:
                        best_tour = new_tour
                        best_len = new_len
                        improved = True
            if not improved: break
        return best_tour

    def evolve(self, generations=100):
        for _ in range(generations):
            # ACO-like pheromone update (simplified)
            lengths = [self._tour_length(t) for t in self.population]
            best_idx = np.argmin(lengths)
            best_tour = self.population[best_idx]

            # Decay
            self.pheromone *= 0.9
            # Reinforce best
            for i in range(self.N):
                self.pheromone[best_tour[i], best_tour[(i+1)%self.N]] += 1.0 / lengths[best_idx]

            # Selection & Crossover (simplified)
            new_pop = [best_tour.copy()]
            for _ in range(self.pop_size - 1):
                # Random swap mutation
                child = best_tour.copy()
                if np.random.rand() < 0.1:
                    i, j = np.random.choice(self.N, 2, replace=False)
                    child[i], child[j] = child[j], child[i]
                new_pop.append(child)
            self.population = new_pop

    def solve(self):
        self.evolve()
        best_tour = self.population[np.argmin([self._tour_length(t) for t in self.population])]
        return self.local_search_2opt(best_tour)
