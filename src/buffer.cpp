#include "protolib/buffer.hpp"

namespace protolib {

Buffer::Buffer() : m_data(nullptr), m_size(0), m_capacity(0) {}
Buffer::~Buffer() { delete[] m_data; }

float* Buffer::data() { return m_data; }
const float* Buffer::data() const { return m_data; }

std::size_t Buffer::size() const { return m_size; }
std::size_t Buffer::capacity() const { return m_capacity; }

void Buffer::reserve(std::size_t) {}
void Buffer::resize(std::size_t) {}

}