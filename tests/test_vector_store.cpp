#include "protolib/vector_store.hpp"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

using protolib::VectorStore;

int main() {
    {
        bool threw = false;
        try {
            VectorStore store(0);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    {
        VectorStore store(4);
        assert(store.dim() == 4);
        assert(store.size() == 0);
        assert(store.capacity() == 0);
    }

    {
        VectorStore store(4);
        std::vector<float> data = {1,2,3,4,5,6,7,8};
        store.add(data, 2);

        assert(store.size() == 2);
        assert(store.capacity() >= 2);
    }

    {
        VectorStore store(4);
        bool threw = false;
        try {
            std::vector<float> bad = {1,2,3,4,5};
            store.add(bad, 2);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    {
        VectorStore store(4);
        std::vector<float> a = {1,2,3,4};
        std::vector<float> b = {5,6,7,8,9,10,11,12};

        store.add(a, 1);
        std::size_t cap1 = store.capacity();

        store.add(b, 2);
        assert(store.size() == 3);
        assert(store.capacity() >= 3);
        assert(store.capacity() >= cap1);
    }

    {
        VectorStore store(4);
        store.reserve(10);

        const float* p1 = store.raw_data();

        std::vector<float> data = {1,2,3,4,5,6,7,8};
        store.add(data, 2);

        const float* p2 = store.raw_data();
        assert(p1 == p2);  // no reallocation expected
    }	

    std::cout << "test_vector_store passed\n";
    return 0;
}
