# Week 4 Progress

## Goals
- implement the L2 (Euclidean) distance kernel in C++
- write unit tests comparing C++ results with manual calculations
- verify compatibility with the contiguous buffer layout

## Completed
- implemented the L2 distance kernel in `src/distance.cpp`
- added the public function declaration in `include/protolib/distance.hpp`
- integrated the distance module into the CMake build
- added GTest-based unit tests for manual distance cases
- verified zero distance, symmetry, negative-value handling, and several hand-calculated examples
- verified that the L2 kernel works correctly with the contiguous vector layout
- verified compatibility with vectors stored inside `VectorStore`

## Notes
- the current implementation returns Euclidean distance, not squared L2 distance
- the kernel assumes valid input pointers and a valid dimension
- this module is the first numerical calculation component of ProtoLib

## Next Step
- implement cosine similarity
- implement brute-force nearest neighbor search
- connect the distance kernel to the search pipeline
