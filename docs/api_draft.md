# API Draft

## C++ API

```cpp
class VectorStore {
public:
    explicit VectorStore(size_t dimension);

    void add(const std::vector<float>& vectors, size_t n_vectors);

    std::pair<std::vector<size_t>, std::vector<float>>
    search_l2(const std::vector<float>& query, size_t k) const;

    std::pair<std::vector<size_t>, std::vector<float>>
    search_cosine(const std::vector<float>& query, size_t k) const;

    size_t size() const;
    size_t dim() const;
};
```
## Constructor

`VectorStore(size_t dimension)`

- `dimension` must be greater than 0
- initializes an empty store
- the vector dimension is fixed after construction

## Add

`add(const std::vector<float>& vectors, size_t n_vectors)`

- input size must satisfy `vectors.size() == n_vectors * dim`
- appends vectors into internal contiguous storage
- automatically expands storage if capacity is insufficient
- throws `std::invalid_argument` on size mismatch

## Search L2

`search_l2(const std::vector<float>& query, size_t k) const`

- query dimension must match store dimension
- `k` must be greater than 0
- store must not be empty
- returns up to `min(k, size())` results
- results are sorted in ascending distance order
- current draft assumes the return values are squared Euclidean distances

## Search Cosine

`search_cosine(const std::vector<float>& query, size_t k) const`

- query dimension must match store dimension
- `k` must be greater than 0
- store must not be empty
- returns up to `min(k, size())` results
- results are sorted in descending similarity order

## Return Format

Both search functions return:

- `ids`: indices of the matched vectors in internal storage
- `values`: corresponding distances or similarity scores

## Python API

```python
store = protolib.VectorStore(dim=128)
store.add(vectors)              # vectors shape: (n, dim)
ids, vals = store.search_l2(query, k=5)
ids, vals = store.search_cosine(query, k=5)
```

## Python Input Rules

- `vectors` should have shape `(n, dim)`
- `query` should have shape `(dim,)`
- dtype is expected to be `float32`

## Error Handling Policy

- invalid dimension at construction -> `std::invalid_argument`
- invalid input length in `add()` -> `std::invalid_argument`
- dimension mismatch in search -> `std::invalid_argument`
- `k == 0` -> `std::invalid_argument`
- empty store search -> `std::runtime_error`

## Ownership Policy

- the vector store owns its internal memory
- input vectors are copied into internal contiguous storage during insertion
- queries are treated as read-only input