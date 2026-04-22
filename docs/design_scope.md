# ProtoLib Scope (Week 1)

## In Scope
- In-memory vector store
- Fixed-dimensional float32 vectors
- Dynamic append
- Exact brute-force search
- L2 distance
- Cosine similarity
- C++11 core
- Python binding with pybind11
- Unit tests for correctness

## Out of Scope
- Approximate nearest neighbor search
- Vector deletion
- On-disk persistence
- Multi-thread-safe insertion
- Distributed storage
- GPU acceleration
- Full continual learning framework

## Design Principle
ProtoLib is designed for moderate-scale research workloads where simplicity, transparency, and low-overhead integration with Python are more important than large-scale distributed indexing.