#include <gtest/gtest.h>

#include "Core/Primitives/RingBuffer.h"

using namespace Voxium::Core;

TEST(RingBufferTest, EnqueueAndDequeueWorks)
{
    RingBuffer<int> rb(5);
    EXPECT_TRUE(rb.IsEmpty());
    EXPECT_EQ(rb.Capacity(), 5);
    EXPECT_EQ(rb.Count(), 0);

    EXPECT_TRUE(rb.Enqueue(10));
    EXPECT_EQ(rb.Count(), 1);
    EXPECT_FALSE(rb.IsEmpty());

    int value = 0;
    EXPECT_TRUE(rb.Dequeue(value));
    EXPECT_EQ(value, 10);
    EXPECT_EQ(rb.Count(), 0);
    EXPECT_TRUE(rb.IsEmpty());
}

TEST(RingBufferTest, FullBufferTest)
{
    RingBuffer<int> rb(3);
    EXPECT_TRUE(rb.Enqueue(1));
    EXPECT_TRUE(rb.Enqueue(2));
    EXPECT_TRUE(rb.Enqueue(3));
    EXPECT_TRUE(rb.IsFull());
    EXPECT_FALSE(rb.Enqueue(4));
}

TEST(RingBufferTest, DequeueEnqueueTest)
{
    RingBuffer<int> rb(3);
    rb.Enqueue(1);
    rb.Enqueue(2);
    rb.Enqueue(3);

    int oldVal = rb.DequeueEnqueue(4);
    EXPECT_EQ(oldVal, 1);
    EXPECT_EQ(rb.Count(), 3);
    EXPECT_EQ(rb[0], 2);
    EXPECT_EQ(rb[1], 3);
    EXPECT_EQ(rb[2], 4);
}

TEST(RingBufferTest, FillTest)
{
    RingBuffer<int> rb(4);
    rb.Fill(7);
    EXPECT_TRUE(rb.IsFull());
    for (int i = 0; i < rb.Count(); ++i)
    {
        EXPECT_EQ(rb[i], 7);
    }
}
