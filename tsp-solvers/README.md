# TSP Solver System

Production-grade, dual-engine Travelling Salesperson Problem solver.

## Engines

### Engine 1: Discrete Memetic Swarm (CPU-first)
- ACO + GA + Local Search
- C++20, OpenMP, CUDA

### Engine 2: Continuous Elastic Ring Projection (GPU-first)
- GNN Embedding + Elastic Ring PDE
- CUDA C++17, PyTorch, TensorRT

## Project Structure

- `common/`: Shared interfaces and types.
- `approach1_memetic/`: Implementation of the memetic swarm engine.
- `approach2_elastic_ring/`: Implementation of the elastic ring engine.
- `prototype_numpy/`: Pure NumPy reference implementations and visualizer.
- `problems/`: Problem loaders for TSPLIB, logistics, and EDA.

## Getting Started

### Prerequisites
- CMake 3.18+
- CUDA Toolkit (for production engines)
- Python 3.11+ with NumPy and Matplotlib (for prototypes)

### Running Prototypes
```bash
export PYTHONPATH=$PYTHONPATH:$(pwd)/tsp-solvers/prototype_numpy
python3 tsp-solvers/prototype_numpy/correctness_check.py
```

### Building C++ Engines
```bash
mkdir build && cd build
cmake ..
make
```
