#include "protolib/vector_store.hpp"

#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using protolib::VectorStore;
using Clock = std::chrono::high_resolution_clock;

std::vector<float> make_random_vectors(std::size_t n, std::size_t dim, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

    std::vector<float> data(n * dim);
    for (auto& x : data) {
        x = dist(rng);
    }
    return data;
}

int main() {
    const std::size_t dim = 128;
    const std::size_t n_vectors = 100000;
    const std::size_t n_queries = 100;
    const std::size_t k = 10;

    VectorStore store(dim);

    auto data = make_random_vectors(n_vectors, dim, 42);
    store.add(data, n_vectors);

    auto queries = make_random_vectors(n_queries, dim, 123);

    {
        auto start = Clock::now();
        for (std::size_t i = 0; i < n_queries; ++i) {
            std::vector<float> q(
                queries.begin() + i * dim,
                queries.begin() + (i + 1) * dim
            );
            auto result = store.search_l2(q, k);
            (void)result;
        }
        auto end = Clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "L2 total time: " << ms << " ms\n";
    }

    {
        auto start = Clock::now();
        for (std::size_t i = 0; i < n_queries; ++i) {
            std::vector<float> q(
                queries.begin() + i * dim,
                queries.begin() + (i + 1) * dim
            );
            auto result = store.search_cosine(q, k);
            (void)result;
        }
        auto end = Clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cout << "Cosine total time: " << ms << " ms\n";
    }

    return 0;
}
