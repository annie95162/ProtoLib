#include "protolib/distance.hpp"
#include <gtest/gtest.h>
#include <cmath>
#include "protolib/distance.hpp"
#include "protolib/vector_store.hpp"

using protolib::l2_distance;
using protolib::VectorStore;

TEST(DistanceTest, ZeroDistanceForIdenticalVectors) {
    float a[] = {1.0f, 2.0f, 3.0f};
    float b[] = {1.0f, 2.0f, 3.0f};

    EXPECT_FLOAT_EQ(l2_distance(a, b, 3), 0.0f);
}

TEST(DistanceTest, OneDimensionalDistance) {
    float a[] = {1.0f};
    float b[] = {4.0f};

    EXPECT_FLOAT_EQ(l2_distance(a, b, 1), 3.0f);
}

TEST(DistanceTest, TwoDimensionalManualCase) {
    float a[] = {1.0f, 1.0f};
    float b[] = {4.0f, 5.0f};

    // sqrt((4-1)^2 + (5-1)^2) = sqrt(9 + 16) = 5
    EXPECT_FLOAT_EQ(l2_distance(a, b, 2), 5.0f);
}

TEST(DistanceTest, ThreeDimensionalManualCase) {
    float a[] = {1.0f, 2.0f, 3.0f};
    float b[] = {4.0f, 6.0f, 3.0f};

    // sqrt((4-1)^2 + (6-2)^2 + (3-3)^2) = sqrt(9 + 16 + 0) = 5
    EXPECT_FLOAT_EQ(l2_distance(a, b, 3), 5.0f);
}

TEST(DistanceTest, NegativeValues) {
    float a[] = {-1.0f, -2.0f};
    float b[] = {2.0f, 2.0f};

    // sqrt((2+1)^2 + (2+2)^2) = sqrt(9 + 16) = 5
    EXPECT_FLOAT_EQ(l2_distance(a, b, 2), 5.0f);
}

TEST(DistanceTest, Symmetry) {
    float a[] = {1.0f, 3.0f, 5.0f};
    float b[] = {2.0f, 4.0f, 8.0f};

    EXPECT_FLOAT_EQ(l2_distance(a, b, 3), l2_distance(b, a, 3));
}

TEST(DistanceTest, WorksWithContiguousVectorLayout) {
    // Two 3D vectors stored contiguously
    float storage[] = {
        1.0f, 2.0f, 3.0f,   // vector 0
        4.0f, 6.0f, 3.0f    // vector 1
    };

    const float* v0 = storage;
    const float* v1 = storage + 3;

    EXPECT_FLOAT_EQ(l2_distance(v0, v1, 3), 5.0f);
}

TEST(VectorStoreTest, L2KernelWorksWithStoredVectors) {
    VectorStore store(3);

    std::vector<float> data = {
        1.0f, 2.0f, 3.0f,
        4.0f, 6.0f, 3.0f
    };

    store.add(data, 2);

    const float* base = store.raw_data();
    ASSERT_NE(base, nullptr);

    const float* v0 = base;
    const float* v1 = base + 3;

    EXPECT_FLOAT_EQ(protolib::l2_distance(v0, v1, 3), 5.0f);
}
