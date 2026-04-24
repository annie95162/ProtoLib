# Week 1 Notes

## Edge Cases

### Constructor
- `dimension = 0` -> throw `std::invalid_argument`

### Add
- invalid flattened size -> throw `std::invalid_argument`
- `n_vectors = 0` -> allowed, no-op

### Search
- empty store -> throw `std::runtime_error`
- query dimension mismatch -> throw `std::invalid_argument`
- `k = 0` -> throw `std::invalid_argument`
- `k > size` -> return all stored vectors

## Cosine Similarity Policy
- zero-norm query -> throw `std::invalid_argument`
- zero-norm stored vector -> similarity = 0

## L2 Distance Policy
Current draft uses **squared Euclidean distance** instead of Euclidean distance with square root.

Reason:
- avoids unnecessary `sqrt`
- preserves ranking order for nearest neighbor search
- simpler and more efficient for the first implementation

## Internal Storage Plan
- row-major contiguous memory layout
- each vector occupies a continuous block of length `dim`
- vector `i` starts at offset `i * dim`

## Capacity Growth Policy
Recommended policy:
- start from capacity = 0
- when more space is needed, grow geometrically
- use `new_capacity = max(1, old_capacity * 2)`
- keep expanding until `new_capacity >= required_capacity`

## Testing Plan

### Storage Tests
- new store has size 0
- add one batch of vectors
- add multiple batches
- verify insertion order after resizing
- verify stored values remain unchanged after growth

### Distance Tests
- verify L2 on hand-crafted vectors
- verify cosine on orthogonal vectors
- verify cosine on identical vectors
- verify zero-vector policy

### Search Tests
- verify top-k results on a small fixed dataset
- verify sorting order of returned results
- verify `k > size` behavior
- verify invalid input handling
- verify dimension mismatch handling

### Python Binding Tests
- verify NumPy shape checking
- verify dtype checking
- verify returned result length
- compare outputs with a NumPy reference implementation

## Initial Baseline Plan
Reference correctness will be checked against NumPy:
- L2: elementwise difference and sum of squares
- cosine: normalized dot product

## Open Questions
- whether Python binding should accept only `float32` or allow implicit conversion
- whether `n_vectors = 0` should remain a no-op in the final version
- whether future versions should support single-vector insertion in Python with shape `(dim,)`

## Week 1 Deliverables
By the end of Week 1, the project should have:
- a fixed scope definition
- storage layout documentation
- API draft
- edge case policy
- testing plan
- repository skeleton for implementation