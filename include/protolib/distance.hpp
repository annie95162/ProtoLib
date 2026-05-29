#ifndef PROTOLIB_DISTANCE_HPP
#define PROTOLIB_DISTANCE_HPP

#include <cstddef>

namespace protolib {

float l2_distance(const float* a, const float* b, std::size_t dim);
float dot_product(const float* a, const float* b, std::size_t dim);
float l2_norm(const float* a, std::size_t dim);
float cosine_similarity(const float* a, const float* b, std::size_t dim);

}

#endif
