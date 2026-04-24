#include "protolib/vector_store.hpp"

namespace protolib {

VectorStore::VectorStore(std::size_t dimension)
    : m_dim(dimension), m_size(0), m_capacity(0), m_data(nullptr) {}

void VectorStore::add(const std::vector<float>&, std::size_t) {}

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

void VectorStore::reserve(std::size_t) {}

const float* VectorStore::vector_ptr(std::size_t) const { return nullptr; }
float* VectorStore::vector_ptr(std::size_t) { return nullptr; }

}