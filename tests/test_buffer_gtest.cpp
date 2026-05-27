#include "protolib/buffer.hpp"
#include <gtest/gtest.h>

using protolib::Buffer;

TEST(BufferTest, DefaultConstruction) {
    Buffer buf;
    EXPECT_EQ(buf.data(), nullptr);
    EXPECT_EQ(buf.size(), 0u);
    EXPECT_EQ(buf.capacity(), 0u);
}

TEST(BufferTest, ReserveAllocatesMemory) {
    Buffer buf;
    buf.reserve(10);

    EXPECT_NE(buf.data(), nullptr);
    EXPECT_EQ(buf.size(), 0u);
    EXPECT_GE(buf.capacity(), 10u);
}

TEST(BufferTest, ResizeUpdatesSize) {
    Buffer buf;
    buf.resize(5);

    EXPECT_NE(buf.data(), nullptr);
    EXPECT_EQ(buf.size(), 5u);
    EXPECT_GE(buf.capacity(), 5u);
}

TEST(BufferTest, ReservePreservesData) {
    Buffer buf;
    buf.resize(3);
    float* p = buf.data();
    p[0] = 1.0f;
    p[1] = 2.0f;
    p[2] = 3.0f;

    buf.reserve(10);

    ASSERT_NE(buf.data(), nullptr);
    EXPECT_EQ(buf.size(), 3u);
    EXPECT_GE(buf.capacity(), 10u);
    EXPECT_FLOAT_EQ(buf.data()[0], 1.0f);
    EXPECT_FLOAT_EQ(buf.data()[1], 2.0f);
    EXPECT_FLOAT_EQ(buf.data()[2], 3.0f);
}
