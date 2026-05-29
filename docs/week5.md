# Week 5 Progress

## Goals
- implement dot product and normalization logic for cosine similarity
- handle zero-vector edge cases
- add unit tests for diverse vector inputs

## Completed
- implemented `dot_product()` in `src/distance.cpp`
- implemented `l2_norm()` in `src/distance.cpp`
- implemented `cosine_similarity()` in `src/distance.cpp`
- added unit tests for dot product, norm, and cosine similarity
- tested identical, orthogonal, opposite, scaled, and mixed-value vectors
- added zero-vector edge case tests
- verified that cosine similarity works with the contiguous vector layout
- passed all tests in both normal and AddressSanitizer builds

## Notes
- cosine similarity is undefined for zero vectors in the current design
- the implementation throws `std::invalid_argument` when either input vector has zero norm
- floating-point comparisons use `EXPECT_NEAR` where appropriate

## Next Step
- connect L2 distance and cosine similarity to brute-force search
- implement top-k result selection
- build the first full search pipeline
