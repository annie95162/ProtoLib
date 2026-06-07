# Week 7 Progress

## Goals
- conduct end-to-end tests for the full C++ pipeline
- verify outputs against a small reference implementation
- fix integration bugs
- stabilize the standalone C++ library

## Completed
- added end-to-end GTest cases for the full VectorStore pipeline
- verified `add()`, `search_l2()`, and `search_cosine()` together on fixed datasets
- validated results against a small NumPy-style reference implementation
- confirmed deterministic tie-breaking by index
- verified zero-query-vector rejection in cosine search
- verified stored zero vectors receive cosine score 0
- ran the full test suite in both normal and ASan builds
- fixed integration-level behavior inconsistencies where needed

## Notes
- the C++ standalone pipeline is now stable for fixed-dimensional vector insertion and brute-force search
- the current search implementation uses `std::partial_sort` for top-k selection
- cosine similarity uses cached stored-vector norms to avoid repeated normalization work

## Next Step
- expose the library to Python using pybind11
- add a NumPy-facing interface
- implement zero-copy data view for Python users
