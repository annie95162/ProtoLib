#include "protolib/distance.hpp"

#include <cmath>
#include <stdexcept>

namespace protolib {

float l2_distance(const float* a, const float* b, std::size_t dim) {
    float sum_sq = 0.0f;

    for (std::size_t i = 0; i < dim; ++i) {
        float diff = a[i] - b[i];
        sum_sq += diff * diff;
    }

    return std::sqrt(sum_sq);
}

float dot_product(const float* a, const float* b, std::size_t dim) {
    float sum = 0.0f;

    for (std::size_t i = 0; i < dim; ++i) {
        sum += a[i] * b[i];
    }

    return sum;
}

float l2_norm(const float* a, std::size_t dim) {
    float sum_sq = 0.0f;

    for (std::size_t i = 0; i < dim; ++i) {
        sum_sq += a[i] * a[i];
    }

    return std::sqrt(sum_sq);
}

float cosine_similarity(const float* a, const float* b, std::size_t dim) {
    float norm_a = l2_norm(a, dim);
    float norm_b = l2_norm(b, dim);

    if (norm_a == 0.0f || norm_b == 0.0f) {
        throw std::invalid_argument("cosine similarity is undefined for zero vectors");
    }

    float dot = dot_product(a, b, dim);
    return dot / (norm_a * norm_b);
}

}  // namespace protolib
