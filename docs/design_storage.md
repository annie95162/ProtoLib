# Storage Layout

Each vector is stored in row-major contiguous memory.

For a store with:
- dimension = d
- number of vectors = n

the internal memory layout is:

[v0_0, v0_1, ..., v0_(d-1),
 v1_0, v1_1, ..., v1_(d-1),
 ...
 v(n-1)_0, ..., v(n-1)_(d-1)]

Vector i starts at:

offset = i * dim

## Internal Fields
- `m_dim`: vector dimension
- `m_size`: current number of stored vectors
- `m_capacity`: allocated number of vectors
- `m_data`: pointer to contiguous float buffer

## Growth Policy
When insertion would exceed the current capacity, the buffer grows geometrically.
Recommended policy:

- new_capacity = max(1, old_capacity * 2)
- keep growing until new_capacity >= required_capacity

## Ownership
The vector store owns its internal storage.
External input data may be copied into the internal buffer during insertion.