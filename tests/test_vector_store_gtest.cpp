#include "protolib/vector_store.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>

using protolib::VectorStore;

TEST(VectorStoreTest, RejectZeroDimension) {
    EXPECT_THROW(VectorStore store(0), std::invalid_argument);
}

TEST(VectorStoreTest, InitialState) {
    VectorStore store(4);
    EXPECT_EQ(store.dim(), 4u);
    EXPECT_EQ(store.size(), 0u);
    EXPECT_EQ(store.capacity(), 0u);
}

TEST(VectorStoreTest, AddSingleBatch) {
    VectorStore store(4);
    std::vector<float> data = {1,2,3,4,5,6,7,8};

    store.add(data, 2);

    EXPECT_EQ(store.size(), 2u);
    EXPECT_GE(store.capacity(), 2u);
}

TEST(VectorStoreTest, RejectInvalidInputLength) {
    VectorStore store(4);
    std::vector<float> bad = {1,2,3,4,5};

    EXPECT_THROW(store.add(bad, 2), std::invalid_argument);
}

TEST(VectorStoreTest, AddMultipleBatches) {
    VectorStore store(4);

    std::vector<float> a = {1,2,3,4};
    std::vector<float> b = {5,6,7,8,9,10,11,12};

    store.add(a, 1);
    std::size_t cap1 = store.capacity();

    store.add(b, 2);

    EXPECT_EQ(store.size(), 3u);
    EXPECT_GE(store.capacity(), 3u);
    EXPECT_GE(store.capacity(), cap1);
}

TEST(VectorStoreTest, AddZeroVectorsIsNoOp) {
    VectorStore store(4);
    std::vector<float> empty;

    store.add(empty, 0);

    EXPECT_EQ(store.size(), 0u);
    EXPECT_EQ(store.capacity(), 0u);
}

TEST(VectorStoreTest, ReserveKeepsPointerStableWhenNoReallocationOccurs) {
    VectorStore store(4);
    store.reserve(10);

    const float* p1 = store.raw_data();

    std::vector<float> data = {1,2,3,4,5,6,7,8};
    store.add(data, 2);

    const float* p2 = store.raw_data();
    EXPECT_EQ(p1, p2);
}

TEST(VectorStoreTest, DataPointerExistsAfterInsertion) {
    VectorStore store(4);
    std::vector<float> data = {1,2,3,4};

    store.add(data, 1);

    EXPECT_NE(store.raw_data(), nullptr);
}

TEST(VectorStoreTest, ReallocationPreservesExistingData) {
    VectorStore store(2);

    std::vector<float> a = {1, 2};
    std::vector<float> b = {3, 4, 5, 6, 7, 8};

    store.add(a, 1);
    store.add(b, 3);

    EXPECT_EQ(store.size(), 4u);
    ASSERT_NE(store.raw_data(), nullptr);
    EXPECT_FLOAT_EQ(store.raw_data()[0], 1.0f);
    EXPECT_FLOAT_EQ(store.raw_data()[1], 2.0f);
}
