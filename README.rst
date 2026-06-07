ProtoLib
========

ProtoLib is a lightweight C++11 and Python library for incremental vector storage and similarity search.

The project is designed for moderate-scale research and educational use cases, especially in machine learning scenarios such as incremental learning, prototype-based inference, and nearest neighbor retrieval.

Unlike large-scale libraries such as FAISS or Milvus, ProtoLib focuses on:

- simplicity
- transparency
- hackability
- efficient contiguous memory storage
- clean Python interoperability through pybind11

Features
--------

- Fixed-dimensional vector storage
- Incremental insertion of vectors
- Exact brute-force top-k search
- L2 (Euclidean) distance search
- Cosine similarity search
- Contiguous memory layout with a custom raw buffer
- Cached vector norms for faster cosine search
- Python bindings with pybind11
- Zero-copy NumPy view of internal storage through ``data_view()``
- Unit tests with Google Test
- Python interface tests with pytest
- Optional OpenMP acceleration for search loops

Project Structure
-----------------

::

   ProtoLib/
   ├── include/protolib/
   │   ├── buffer.hpp
   │   ├── distance.hpp
   │   └── vector_store.hpp
   ├── src/
   │   ├── buffer.cpp
   │   ├── distance.cpp
   │   └── vector_store.cpp
   ├── python/
   │   └── binding.cpp
   ├── tests/
   │   ├── python/
   │   │   └── test_python_api.py
   │   ├── test_buffer.cpp
   │   ├── test_buffer_gtest.cpp
   │   ├── test_distance_gtest.cpp
   │   ├── test_pipeline_gtest.cpp
   │   ├── test_vector_store.cpp
   │   └── test_vector_store_gtest.cpp
   ├── benchmarks/
   │   └── benchmark_search.cpp
   ├── examples/
   │   ├── demo.cpp
   │   └── demo.py
   ├── reference/
   │   └── reference_search_check.py
   ├── docs/
   ├── README.rst
   └── CMakeLists.txt

Build Instructions
------------------

C++ build
~~~~~~~~~

::

   mkdir build
   cd build
   cmake ..
   make

Run C++ tests
~~~~~~~~~~~~~

::

   ctest --output-on-failure

Build with OpenMP
~~~~~~~~~~~~~~~~~

::

   mkdir build_omp
   cd build_omp
   cmake -DENABLE_OPENMP=ON ..
   make

Python module
~~~~~~~~~~~~~

After building, the Python extension module is generated in the build directory.

A simple import test:

::

   cd build
   python3 -c "import sys; sys.path.insert(0, '.'); import protolib; print('ok')"

Python Requirements
-------------------

- Python 3
- NumPy
- pytest
- pybind11

Python Usage Example
--------------------

::

   import numpy as np
   import protolib

   store = protolib.VectorStore(3)

   vectors = np.array([
       [1.0, 2.0, 3.0],
       [4.0, 5.0, 6.0],
       [1.0, 0.0, 0.0],
   ], dtype=np.float32)

   store.add(vectors)

   query = np.array([1.0, 2.0, 3.0], dtype=np.float32)

   ids_l2, vals_l2 = store.search_l2(query, 2)
   ids_cos, vals_cos = store.search_cosine(query, 2)

   print("L2 ids:", ids_l2)
   print("L2 values:", vals_l2)
   print("Cosine ids:", ids_cos)
   print("Cosine values:", vals_cos)

Zero-Copy NumPy View
--------------------

ProtoLib provides a zero-copy NumPy view of internal storage through ``data_view()``.

::

   view = store.data_view()
   print(view.shape)
   print(view.dtype)
   print(view)

The returned array is a NumPy view of the internal contiguous storage, so no extra copy is made for reading.

Design Overview
---------------

ProtoLib uses a layered design:

- ``Buffer`` manages low-level raw contiguous memory
- ``VectorStore`` provides vector insertion and search APIs
- ``distance`` contains numerical kernels such as L2 distance, dot product, norm, and cosine similarity
- ``binding.cpp`` exposes the library to Python through pybind11

Vectors are stored in row-major contiguous layout. If the vector dimension is ``dim``, then the i-th vector begins at offset ``i * dim``.

Current Search Strategy
-----------------------

ProtoLib currently uses exact brute-force search.

For each query:

- all stored vectors are scanned
- L2 distance or cosine similarity is computed
- top-k results are selected with ``std::partial_sort``

This design emphasizes correctness, transparency, and moderate-scale efficiency.

Performance
-----------

ProtoLib includes a benchmark program for evaluating search performance.

::

   cd build_omp
   ./benchmark_search

The search loops can be parallelized with OpenMP when enabled.

Testing
-------

C++ tests
~~~~~~~~~

ProtoLib includes both basic and Google Test based C++ tests.

Examples:

::

   ./test_buffer
   ./test_vector_store
   ./test_buffer_gtest
   ./test_vector_store_gtest
   ./test_distance_gtest
   ./test_pipeline_gtest

Python tests
~~~~~~~~~~~~

If the Python extension has been built in the ``build`` directory:

::

   cd build
   export PYTHONPATH=$(pwd):$PYTHONPATH
   pytest ../tests/python -v

Reference Verification
----------------------

A small reference script is provided in ``reference/reference_search_check.py`` for checking search behavior against a NumPy-style implementation on small test cases.

This reference was mainly used to verify correctness and integration behavior during development.

Demo
----

A demo script is provided in ``examples/demo_incremental_learning.py``.

It demonstrates ProtoLib in a simple incremental learning scenario:

- class prototypes are stored in memory
- retrieval is performed with L2 distance and cosine similarity
- a new class prototype is inserted incrementally
- retrieval results are updated immediately after memory expansion

This illustrates how ProtoLib can serve as a lightweight prototype memory for incremental learning and nearest-prototype inference.

Run the Demo
------------

If the Python module has been built in the ``build`` directory, the demo can be run with:

::

   PYTHONPATH=build python3 examples/demo.py

Limitations
-----------

- Exact brute-force search only
- No approximate nearest neighbor indexing
- No deletion of stored vectors
- No disk-backed persistence
- No distributed storage

These limitations are intentional in order to keep the implementation small, transparent, and suitable for research and educational purposes.

Use Cases
---------

ProtoLib is suitable for:

- incremental learning experiments
- prototype-based classification
- nearest neighbor retrieval
- embedding storage and lookup
- educational demonstrations of vector search systems

Author
------

ProtoLib was developed as a numerical software development final project focusing on efficient vector storage, similarity search, Python interoperability, and performance optimization.
