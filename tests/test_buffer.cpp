#include "protolib/buffer.hpp"

#include <cassert>
#include <iostream>

using protolib::Buffer;

int main() {
    {
        Buffer buf;
        assert(buf.data() == nullptr);
        assert(buf.size() == 0);
        assert(buf.capacity() == 0);
    }

    {
        Buffer buf;
        buf.reserve(10);
        assert(buf.capacity() >= 10);
        assert(buf.size() == 0);
        assert(buf.data() != nullptr);
    }

    {
        Buffer buf;
        buf.resize(5);
        assert(buf.size() == 5);
        assert(buf.capacity() >= 5);
        assert(buf.data() != nullptr);
    }

    {
        Buffer buf;
        buf.resize(3);
        float* p = buf.data();
        p[0] = 1.0f;
        p[1] = 2.0f;
        p[2] = 3.0f;

        buf.reserve(10);
        assert(buf.size() == 3);
        assert(buf.capacity() >= 10);
        assert(buf.data()[0] == 1.0f);
        assert(buf.data()[1] == 2.0f);
        assert(buf.data()[2] == 3.0f);
    }

    std::cout << "test_buffer passed\n";
    return 0;
}
