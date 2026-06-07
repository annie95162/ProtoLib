# Week 8 Progress

## Goals
- write pybind11 glue code for `VectorStore`
- provide efficient NumPy access
- write pytest scripts for the Python-C++ interface

## Completed
- added pybind11 bindings for `VectorStore`
- exposed constructor, `add`, `search_l2`, `search_cosine`, `size`, `dim`, and `capacity`
- added strict NumPy input validation for dtype, dimensionality, and C-contiguous layout
- implemented `add_raw()` so Python insertion performs only one controlled copy into internal storage
- implemented `data_view()` as a zero-copy NumPy view of internal storage
- added pytest coverage for insertion, search, invalid inputs, and NumPy view behavior

## Notes
- Python insertion is not zero-copy because the store owns its internal memory
- zero-copy is provided for reading internal data through `data_view()`
- the Python API intentionally rejects non-float32 and non-contiguous inputs to avoid hidden copies

## Next Step
- profile the numerical kernels and search pipeline
- parallelize distance loops with OpenMP
- compare performance before and after optimization
