import numpy as np

class ElasticRingSolver:
    def __init__(self, nodes, gamma=1.5, alpha=1.0, beta=1.0, beta_b_max=1.0):
        self.nodes = nodes  # (N, D)
        self.N = len(nodes)
        self.M = int(gamma * self.N)
        self.D = nodes.shape[1]
        self.alpha = alpha
        self.beta = beta
        self.beta_b_max = beta_b_max

        # Initialize ring as a small circle around centroid
        centroid = np.mean(nodes, axis=0)
        radius = 0.1 # Start small
        angles = np.linspace(0, 2 * np.pi, self.M, endpoint=False)
        self.Y = centroid + radius * np.stack([np.cos(angles), np.sin(angles)], axis=1)

        self.V = np.zeros_like(self.Y) # Velocity for Nesterov

    def get_forces(self, K):
        # Advective force
        # F_adv(j) = alpha * sum_i w_ij * (x_i - y_j)
        # w_ij = softmax(-||x_i - y_j||^2 / 2K^2)

        # O(N*M)
        diff_ni = self.nodes[:, np.newaxis, :] - self.Y[np.newaxis, :, :] # (N, M, D)
        dist_sq = np.sum(diff_ni**2, axis=2) # (N, M)

        # Stability trick for softmax
        W = np.exp(-(dist_sq - np.min(dist_sq, axis=1, keepdims=True)) / (2 * K**2))
        W /= (np.sum(W, axis=1, keepdims=True) + 1e-12)

        F_adv = self.alpha * np.sum(W[:, :, np.newaxis] * diff_ni, axis=0) # (M, D)

        # Elastic force
        # F_elast(j) = beta * K * (y_{j+1} - 2y_j + y_{j-1})
        Y_next = np.roll(self.Y, -1, axis=0)
        Y_prev = np.roll(self.Y, 1, axis=0)
        F_elast = self.beta * K * (Y_next - 2 * self.Y + Y_prev)

        # Barrier force
        F_barrier = np.zeros_like(self.Y)
        beta_b = self.beta_b_max * (1.0 - K / 1.0)
        if beta_b > 0:
            for j in range(self.M):
                diff = self.Y[j] - self.Y
                dist_sq_ring = np.sum(diff**2, axis=1)
                mask = np.ones(self.M, dtype=bool)
                mask[j] = False
                mask[(j+1)%self.M] = False
                mask[(j-1)%self.M] = False

                inv_dist_sq = 1.0 / (dist_sq_ring[mask] + 1e-6)
                F_barrier[j] = beta_b * np.sum(inv_dist_sq[:, np.newaxis] * diff[mask], axis=0)

        return F_adv + F_elast + F_barrier

    def anneal(self, epochs=2000, K0=1.0, epsilon=0.01):
        K = K0
        for epoch in range(epochs):
            F = self.get_forces(K)

            # Simple Euler for now
            self.Y += 0.1 * F

            K *= (1.0 - epsilon)
            if K < 0.01: break

    def get_tour(self):
        # DTML: BMU mapping
        dist_sq = np.sum((self.nodes[:, np.newaxis, :] - self.Y[np.newaxis, :, :])**2, axis=2)
        bmu_indices = np.argmin(dist_sq, axis=1)

        # The tour is defined by the order of BMUs on the ring
        # For each city i, we have a ring node j_i.
        # We visit cities in order of their j_i.
        city_indices = np.argsort(bmu_indices)
        return city_indices
