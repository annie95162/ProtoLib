#include "protolib/buffer.hpp"
#include <algorithm>
#include <stdexcept>
namespace protolib {

Buffer::Buffer() : m_data(nullptr), m_size(0), m_capacity(0) {}
Buffer::~Buffer() { delete[] m_data; }

Buffer::Buffer(Buffer&& other) noexcept
    : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
    if (this != &other) {
        delete[] m_data;

        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    return *this;
}

float* Buffer::data() { return m_data; }
const float* Buffer::data() const { return m_data; }

std::size_t Buffer::size() const { return m_size; }
std::size_t Buffer::capacity() const { return m_capacity; }

void Buffer::reserve(std::size_t new_capacity) {
    if (new_capacity <= m_capacity) {
        return;
    }

    float* new_data = new float[new_capacity];

    for (std::size_t i = 0; i < m_size; ++i) {
        new_data[i] = m_data[i];
    }

    delete[] m_data;
    m_data = new_data;
    m_capacity = new_capacity;
}
void Buffer::resize(std::size_t new_size) {
    if (new_size > m_capacity) {
        reserve(new_size);
    }
    m_size = new_size;
}

}
