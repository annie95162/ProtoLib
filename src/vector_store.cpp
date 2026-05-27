#include "protolib/vector_store.hpp"
#include <stdexcept>

namespace protolib {

VectorStore::VectorStore(std::size_t dimension)
    : m_dim(dimension), m_size(0), m_capacity(0), m_storage() {
    if (dimension == 0) {
        throw std::invalid_argument("dimension must be greater than 0");
    }
}

void VectorStore::add(const std::vector<float>& vectors, std::size_t n_vectors) {
    if (vectors.size() != n_vectors * m_dim) {
        throw std::invalid_argument("input size does not match n_vectors * dim");
    }

    if (n_vectors == 0) {
        return;
    }

    std::size_t required_size = m_size + n_vectors;

    if (required_size > m_capacity) {
        std::size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity;
        while (new_capacity < required_size) {
            new_capacity *= 2;
        }
        reserve(new_capacity);
    }

    std::size_t old_float_count = m_size * m_dim;
    std::size_t new_float_count = required_size * m_dim;

    m_storage.resize(new_float_count);

    float* dst = m_storage.data() + old_float_count;
    for (std::size_t i = 0; i < vectors.size(); ++i) {
        dst[i] = vectors[i];
    }

    m_size = required_size;
}

std::pair<std::vector<std::size_t>, std::vector<float>>
VectorStore::search_l2(const std::vector<float>&, std::size_t) const {
    return {};
}

std::pair<std::vector<std::size_t>, std::vector<float>>
VectorStore::search_cosine(const std::vector<float>&, std::size_t) const {
    return {};
}

std::size_t VectorStore::size() const { return m_size; }
std::size_t VectorStore::dim() const { return m_dim; }
std::size_t VectorStore::capacity() const {
    return m_capacity;
}
const float* VectorStore::raw_data() const {
    return m_storage.data();
}
void VectorStore::reserve(std::size_t new_capacity) {
    if (new_capacity <= m_capacity) {
        return;
    }

    m_storage.reserve(new_capacity * m_dim);
    m_capacity = new_capacity;
}

const float* VectorStore::vector_ptr(std::size_t idx) const {
    return m_storage.data() + idx * m_dim;
}
float* VectorStore::vector_ptr(std::size_t idx) {
    return m_storage.data() + idx * m_dim;
}

}
