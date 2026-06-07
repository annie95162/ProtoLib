#include "protolib/vector_store.hpp"
#include <gtest/gtest.h>
#include <stdexcept>
#include <vector>
#include <cmath>

using protolib::VectorStore;

TEST(PipelineTest, EndToEndL2SearchMatchesManualReference) {
    VectorStore store(2);

    std::vector<float> data = {
        1.0f, 1.0f,   // idx 0
        4.0f, 5.0f,   // idx 1
        2.0f, 2.0f,   // idx 2
        1.0f, 2.0f    // idx 3
    };

    store.add(data, 4);

    std::vector<float> query = {1.0f, 1.0f};
    auto result = store.search_l2(query, 3);

    ASSERT_EQ(result.first.size(), 3u);
    ASSERT_EQ(result.second.size(), 3u);

    // manual distances:
    // idx0 -> 0
    // idx1 -> 5
    // idx2 -> sqrt(2)
    // idx3 -> 1
    EXPECT_EQ(result.first[0], 0u);
    EXPECT_FLOAT_EQ(result.second[0], 0.0f);

    EXPECT_EQ(result.first[1], 3u);
    EXPECT_FLOAT_EQ(result.second[1], 1.0f);

    EXPECT_EQ(result.first[2], 2u);
    EXPECT_NEAR(result.second[2], std::sqrt(2.0f), 1e-6);
}

TEST(PipelineTest, EndToEndCosineSearchMatchesManualReference) {
    VectorStore store(2);

    std::vector<float> data = {
        1.0f, 0.0f,   // idx 0
        0.0f, 1.0f,   // idx 1
        2.0f, 0.0f,   // idx 2
        1.0f, 1.0f    // idx 3
    };

    store.add(data, 4);

    std::vector<float> query = {1.0f, 0.0f};
    auto result = store.search_cosine(query, 4);

    ASSERT_EQ(result.first.size(), 4u);
    ASSERT_EQ(result.second.size(), 4u);

    // cosine:
    // idx0 = 1
    // idx1 = 0
    // idx2 = 1
    // idx3 = 1/sqrt(2)
    EXPECT_EQ(result.first[0], 0u);
    EXPECT_FLOAT_EQ(result.second[0], 1.0f);

    EXPECT_EQ(result.first[1], 2u);
    EXPECT_FLOAT_EQ(result.second[1], 1.0f);

    EXPECT_EQ(result.first[2], 3u);
    EXPECT_NEAR(result.second[2], 1.0f / std::sqrt(2.0f), 1e-6);

    EXPECT_EQ(result.first[3], 1u);
    EXPECT_FLOAT_EQ(result.second[3], 0.0f);
}

TEST(PipelineTest, L2SearchTieBreaksByIndex) {
    VectorStore store(2);

    std::vector<float> data = {
        2.0f, 0.0f,   // idx 0
        0.0f, 2.0f    // idx 1
    };

    store.add(data, 2);

    std::vector<float> query = {0.0f, 0.0f};
    auto result = store.search_l2(query, 2);

    ASSERT_EQ(result.first.size(), 2u);
    EXPECT_EQ(result.first[0], 0u);
    EXPECT_EQ(result.first[1], 1u);
}

TEST(PipelineTest, CosineSearchTieBreaksByIndex) {
    VectorStore store(2);

    std::vector<float> data = {
        1.0f, 0.0f,   // idx 0
        2.0f, 0.0f    // idx 1
    };

    store.add(data, 2);

    std::vector<float> query = {1.0f, 0.0f};
    auto result = store.search_cosine(query, 2);

    ASSERT_EQ(result.first.size(), 2u);
    EXPECT_EQ(result.first[0], 0u);
    EXPECT_EQ(result.first[1], 1u);
}

TEST(PipelineTest, CosineSearchStoredZeroVectorGetsZeroScore) {
    VectorStore store(2);

    std::vector<float> data = {
        0.0f, 0.0f,   // idx 0
        1.0f, 0.0f    // idx 1
    };

    store.add(data, 2);

    std::vector<float> query = {1.0f, 0.0f};
    auto result = store.search_cosine(query, 2);

    ASSERT_EQ(result.first.size(), 2u);
    EXPECT_EQ(result.first[0], 1u);
    EXPECT_FLOAT_EQ(result.second[0], 1.0f);

    EXPECT_EQ(result.first[1], 0u);
    EXPECT_FLOAT_EQ(result.second[1], 0.0f);
}

TEST(PipelineTest, SearchRejectsInvalidInputs) {
    VectorStore store(2);
    std::vector<float> data = {1.0f, 1.0f};
    store.add(data, 1);

    std::vector<float> bad_query = {1.0f, 2.0f, 3.0f};
    std::vector<float> zero_query = {0.0f, 0.0f};
    std::vector<float> good_query = {1.0f, 1.0f};

    EXPECT_THROW(store.search_l2(bad_query, 1), std::invalid_argument);
    EXPECT_THROW(store.search_cosine(bad_query, 1), std::invalid_argument);
    EXPECT_THROW(store.search_l2(good_query, 0), std::invalid_argument);
    EXPECT_THROW(store.search_cosine(good_query, 0), std::invalid_argument);
    EXPECT_THROW(store.search_cosine(zero_query, 1), std::invalid_argument);
}
