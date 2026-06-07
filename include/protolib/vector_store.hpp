#ifndef PROTOLIB_VECTOR_STORE_HPP
#define PROTOLIB_VECTOR_STORE_HPP

#include <cstddef>
#include <utility>
#include <vector>
#include "protolib/buffer.hpp"
namespace protolib {

class VectorStore {
public:
    explicit VectorStore(std::size_t dimension);

    void add(const std::vector<float>& vectors, std::size_t n_vectors);

    std::pair<std::vector<std::size_t>, std::vector<float>>
    search_l2(const std::vector<float>& query, std::size_t k) const;

    std::pair<std::vector<std::size_t>, std::vector<float>>
    search_cosine(const std::vector<float>& query, std::size_t k) const;

    std::size_t size() const;
    std::size_t dim() const;
    std::size_t capacity() const;
    const float* raw_data() const;

    void reserve(std::size_t new_capacity);
private:
    std::size_t m_dim;
    std::size_t m_size;
    std::size_t m_capacity;
    Buffer m_storage;
    std::vector<float> m_norms;

    const float* vector_ptr(std::size_t idx) const;
    float* vector_ptr(std::size_t idx);
};

}

#endif
