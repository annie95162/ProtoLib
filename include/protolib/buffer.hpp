#ifndef PROTOLIB_BUFFER_HPP
#define PROTOLIB_BUFFER_HPP

#include <cstddef>

namespace protolib {

class Buffer {
public:
    Buffer();
    ~Buffer();

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    float* data();
    const float* data() const;

    std::size_t size() const;
    std::size_t capacity() const;

    void reserve(std::size_t new_capacity);
    void resize(std::size_t new_size);

private:
    float* m_data;
    std::size_t m_size;
    std::size_t m_capacity;
};

}  // namespace protolib

#endif