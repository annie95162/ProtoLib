# Week 3 Progress

## Goals
- set up GTest
- write tests for `add()` and resizing behavior
- check for memory leaks and invalid memory accesses

## Completed
- added Google Test framework to the CMake build
- added GTest-based unit tests for `Buffer`
- added GTest-based unit tests for `VectorStore`
- tested `add()` with valid and invalid inputs
- tested zero-vector insertion behavior
- tested capacity growth and resizing behavior
- tested data preservation after reallocation
- tested pointer stability when no reallocation occurs
- ran AddressSanitizer on the current memory/storage layer
- ran Valgrind to check for memory leaks

## Notes
- pointer stability is only guaranteed when reallocation does not occur
- if the storage grows, the internal raw pointer may change, which is expected
- current tests focus on validating the memory core before implementing search

## Next Step
- implement L2 distance
- implement cosine similarity
- implement brute-force search
- add correctness tests for distance and search
