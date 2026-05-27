#include "protolib/distance.hpp"

#include <cmath>

namespace protolib {

float l2_distance(const float* a, const float* b, std::size_t dim) {
    float sum_sq = 0.0f;

    for (std::size_t i = 0; i < dim; ++i) {
        float diff = a[i] - b[i];
        sum_sq += diff * diff;
    }

    return std::sqrt(sum_sq);
}

}  // namespace protolib
