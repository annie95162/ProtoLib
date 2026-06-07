#include "protolib/vector_store.hpp"
#include "protolib/distance.hpp"
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <vector>
#include <cstdint>
#ifdef _OPENMP
#include <omp.h>
#endif

namespace protolib {

VectorStore::VectorStore(std::size_t dimension)
    : m_dim(dimension), m_size(0), m_capacity(0), m_storage(), m_norms() {
    if (dimension == 0) {
        throw std::invalid_argument("dimension must be greater than 0");
    }
}

void VectorStore::add_raw(const float* vectors, std::size_t n_vectors) {
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

    m_norms.reserve(required_size);

    std::size_t old_float_count = m_size * m_dim;
    std::size_t new_float_count = required_size * m_dim;

    m_storage.resize(new_float_count);

    float* dst = m_storage.data() + old_float_count;
    std::copy(vectors, vectors + n_vectors * m_dim, dst);

    for (std::size_t i = 0; i < n_vectors; ++i) {
        const float* vec_ptr = dst + i * m_dim;
        m_norms.push_back(l2_norm(vec_ptr, m_dim));
    }

    m_size = required_size;
}

void VectorStore::add(const std::vector<float>& vectors, std::size_t n_vectors) {
    if (vectors.size() != n_vectors * m_dim) {
        throw std::invalid_argument("input size does not match n_vectors * dim");
    }

    add_raw(vectors.data(), n_vectors);
}

std::pair<std::vector<std::size_t>, std::vector<float>>
VectorStore::search_l2(const std::vector<float>& query, std::size_t k) const {
    if (query.size() != m_dim) {
        throw std::invalid_argument("query dimension does not match store dimension");
    }

    if (k == 0) {
        throw std::invalid_argument("k must be greater than 0");
    }

    if (m_size == 0) {
        throw std::runtime_error("cannot search an empty store");
    }

    std::vector<std::pair<std::size_t, float>> results(m_size);

    #pragma omp parallel for if(m_size > 1000)
    for (std::int64_t i = 0; i < static_cast<std::int64_t>(m_size); ++i) {
        const float* vec = vector_ptr(static_cast<std::size_t>(i));
        float dist = l2_distance(query.data(), vec, m_dim);
        results[static_cast<std::size_t>(i)] = {
            static_cast<std::size_t>(i), dist
        };
    }

    std::size_t out_k = std::min(k, m_size);

    std::partial_sort(
        results.begin(),
        results.begin() + out_k,
        results.end(),
        [](const std::pair<std::size_t, float>& a,
           const std::pair<std::size_t, float>& b) {
            if (a.second != b.second) {
                return a.second < b.second;
            }
            return a.first < b.first;
        });

    std::vector<std::size_t> ids;
    std::vector<float> values;
    ids.reserve(out_k);
    values.reserve(out_k);

    for (std::size_t i = 0; i < out_k; ++i) {
        ids.push_back(results[i].first);
        values.push_back(results[i].second);
    }

    return {ids, values};
}

std::pair<std::vector<std::size_t>, std::vector<float>>
VectorStore::search_cosine(const std::vector<float>& query, std::size_t k) const {
    if (query.size() != m_dim) {
        throw std::invalid_argument("query dimension does not match store dimension");
    }

    if (k == 0) {
        throw std::invalid_argument("k must be greater than 0");
    }

    if (m_size == 0) {
        throw std::runtime_error("cannot search an empty store");
    }

    float query_norm = l2_norm(query.data(), m_dim);
    if (query_norm == 0.0f) {
        throw std::invalid_argument("cosine search is undefined for a zero query vector");
    }

    std::vector<std::pair<std::size_t, float>> results(m_size);

    #pragma omp parallel for if(m_size > 1000)
    for (std::int64_t i = 0; i < static_cast<std::int64_t>(m_size); ++i) {
        const std::size_t idx = static_cast<std::size_t>(i);
        const float* vec = vector_ptr(idx);
        float vec_norm = m_norms[idx];

        float sim = 0.0f;
        if (vec_norm != 0.0f) {
            float dot = dot_product(query.data(), vec, m_dim);
            sim = dot / (query_norm * vec_norm);
        }

        results[idx] = {idx, sim};
    }

    std::size_t out_k = std::min(k, m_size);

    std::partial_sort(
        results.begin(),
        results.begin() + out_k,
        results.end(),
        [](const std::pair<std::size_t, float>& a,
           const std::pair<std::size_t, float>& b) {
            if (a.second != b.second) {
                return a.second > b.second;
            }
            return a.first < b.first;
        });

    std::vector<std::size_t> ids;
    std::vector<float> values;
    ids.reserve(out_k);
    values.reserve(out_k);

    for (std::size_t i = 0; i < out_k; ++i) {
        ids.push_back(results[i].first);
        values.push_back(results[i].second);
    }

    return {ids, values};
}

/*
std::pair<std::vector<std::size_t>, std::vector<float>>
VectorStore::search_l2(const std::vector<float>& query, std::size_t k) const {
    if (query.size() != m_dim) {
        throw std::invalid_argument("query dimension does not match store dimension");
    }

    if (k == 0) {
        throw std::invalid_argument("k must be greater than 0");
    }

    if (m_size == 0) {
        throw std::runtime_error("cannot search an empty store");
    }

    std::vector<std::pair<std::size_t, float>> results;
    results.reserve(m_size);

    for (std::size_t i = 0; i < m_size; ++i) {
        const float* vec = vector_ptr(i);
        float dist = l2_distance(query.data(), vec, m_dim);
        results.push_back({i, dist});
    }

    std::size_t out_k = std::min(k, m_size);

    std::partial_sort(
        results.begin(),
        results.begin() + out_k,
        results.end(),
        [](const std::pair<std::size_t, float>& a,
           const std::pair<std::size_t, float>& b) {
            if (a.second != b.second) {
                return a.second < b.second;
            }
            return a.first < b.first;
        });

    std::vector<std::size_t> ids;
    std::vector<float> values;
    ids.reserve(out_k);
    values.reserve(out_k);

    for (std::size_t i = 0; i < out_k; ++i) {
        ids.push_back(results[i].first);
        values.push_back(results[i].second);
    }

    return {ids, values};
}
*/

/*
std::pair<std::vector<std::size_t>, std::vector<float>>
VectorStore::search_cosine(const std::vector<float>& query, std::size_t k) const {
    if (query.size() != m_dim) {
        throw std::invalid_argument("query dimension does not match store dimension");
    }

    if (k == 0) {
        throw std::invalid_argument("k must be greater than 0");
    }

    if (m_size == 0) {
        throw std::runtime_error("cannot search an empty store");
    }

    float query_norm = l2_norm(query.data(), m_dim);
    if (query_norm == 0.0f) {
        throw std::invalid_argument("cosine search is undefined for a zero query vector");
    }

    std::vector<std::pair<std::size_t, float>> results;
    results.reserve(m_size);

    for (std::size_t i = 0; i < m_size; ++i) {
        const float* vec = vector_ptr(i);
        float vec_norm = m_norms[i];

        float sim = 0.0f;
        if (vec_norm != 0.0f) {
            float dot = dot_product(query.data(), vec, m_dim);
            sim = dot / (query_norm * vec_norm);
        }

        results.push_back({i, sim});
    }

    std::size_t out_k = std::min(k, m_size);

    std::partial_sort(
        results.begin(),
        results.begin() + out_k,
        results.end(),
        [](const std::pair<std::size_t, float>& a,
           const std::pair<std::size_t, float>& b) {
            if (a.second != b.second) {
                return a.second > b.second;
            }
            return a.first < b.first;
        });

    std::vector<std::size_t> ids;
    std::vector<float> values;
    ids.reserve(out_k);
    values.reserve(out_k);

    for (std::size_t i = 0; i < out_k; ++i) {
        ids.push_back(results[i].first);
        values.push_back(results[i].second);
    }

    return {ids, values};
}
*/

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

float* VectorStore::raw_data() {
    return m_storage.data();
}

std::size_t VectorStore::raw_size() const {
    return m_size * m_dim;
}
}
