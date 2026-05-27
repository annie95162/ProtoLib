# Week 2 Progress

## Goals
- Implement manual memory allocation with raw `float*`
- Handle capacity growth and resizing
- Ensure pointer stability when no reallocation occurs

## Completed
- Implemented a `Buffer` class using raw `float*` with `new[]` and `delete[]`
- Added `reserve()` and `resize()` logic for contiguous storage management
- Integrated `Buffer` into `VectorStore`
- Implemented `add()` for batch vector insertion
- Added `raw_data()` for testing/debugging internal storage pointer behavior
- Added tests for buffer behavior and vector store storage behavior
- Verified that stored data is preserved after reallocation
- Verified that pointer remains stable when no reallocation occurs

## Notes
- Pointer stability is guaranteed only if the underlying storage is not reallocated
- If capacity expansion happens, the internal raw pointer may change, which is expected
- This design is intended to support future Python sharing more safely

## Next Step
- Implement L2 distance and cosine similarity
- Build brute-force top-k search
- Add correctness tests for distance and search functions
