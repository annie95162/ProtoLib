# Week 6 Progress

## Goals
- implement top-k selection logic
- integrate with L2 and cosine kernels
- ensure the search returns both indices and distances/similarity scores
- improve insertion efficiency where appropriate

## Completed
- optimized `add()` using `std::copy`
- added cached vector norms for cosine similarity
- implemented brute-force `search_l2()` in `VectorStore`
- implemented brute-force `search_cosine()` in `VectorStore`
- implemented top-k selection using `std::partial_sort`
- ensured the search functions return both matched indices and corresponding values
- added unit tests for L2 search and cosine search
- tested sorting behavior, invalid inputs, empty store, and `k > size`
- added deterministic tie-breaking by index
- passed tests in both normal and ASan builds

## Notes
- the current top-k selection uses `std::partial_sort`, which is more appropriate than full sorting for this stage
- cosine search uses cached L2 norms of stored vectors to avoid recomputing norms during every query
- zero query vectors are rejected in cosine search
- stored zero vectors are assigned cosine similarity 0

## Next Step
- conduct end-to-end tests for the full C++ pipeline
- verify outputs against a small NumPy reference implementation
- fix any remaining integration bugs
