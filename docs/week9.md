# Week 9 Progress

## Goals
- profile the code to identify numerical bottlenecks
- implement OpenMP parallelization for distance calculation loops
- compare performance before and after optimization

## Completed
- added a benchmark program for the full search pipeline
- measured baseline performance of `search_l2()` and `search_cosine()`
- identified the brute-force distance/similarity loop as the dominant numerical bottleneck
- parallelized the distance computation loop in `search_l2()` using OpenMP
- parallelized the similarity computation loop in `search_cosine()` using OpenMP
- verified correctness after parallelization using the existing C++ test suite
- compared performance before and after optimization

## Notes
- the current implementation parallelizes the per-vector distance/similarity computation stage
- top-k selection remains unchanged and still uses `std::partial_sort`
- the search logic preserves deterministic tie-breaking by index
- correctness was revalidated after optimization

## Result Summary
- baseline L2 time: [fill in]
- OpenMP L2 time: [fill in]
- baseline cosine time: [fill in]
- OpenMP cosine time: [fill in]

## Next Step
- prepare final presentation
- summarize design choices, optimizations, and performance results
