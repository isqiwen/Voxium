#include <gtest/gtest.h>
#include <limits>

#include "Core/Primitives/MovingAverage.h"

using namespace Voxium::Core;

TEST(AccurateMovingAverageTest, AverageCalculation)
{
    AccurateMovingAverage ama(3);
    EXPECT_NEAR(ama.Average(), 0.0, std::numeric_limits<double>::epsilon());

    ama.Add(10.0);
    EXPECT_NEAR(ama.Average(), 10.0, std::numeric_limits<double>::epsilon());

    ama.Add(20.0);
    EXPECT_NEAR(ama.Average(), 15.0, std::numeric_limits<double>::epsilon());

    ama.Add(30.0);
    EXPECT_NEAR(ama.Average(), 20.0, std::numeric_limits<double>::epsilon());

    ama.Add(40.0);
    EXPECT_NEAR(ama.Average(), 30.0, std::numeric_limits<double>::epsilon());
}

TEST(MovingAverageTest, AverageCalculation)
{
    MovingAverage ma(3);
    EXPECT_NEAR(ma.Average(), 0.0, std::numeric_limits<double>::epsilon());

    ma.Add(10.0);
    EXPECT_NEAR(ma.Average(), 10.0, std::numeric_limits<double>::epsilon());

    ma.Add(20.0);
    EXPECT_NEAR(ma.Average(), 15.0, std::numeric_limits<double>::epsilon());

    ma.Add(30.0);
    EXPECT_NEAR(ma.Average(), 20.0, std::numeric_limits<double>::epsilon());

    ma.Add(40.0);
    EXPECT_NEAR(ma.Average(), 30.0, std::numeric_limits<double>::epsilon());
}

TEST(MovingAverageTest, EnsureAndResize)
{
    MovingAverage ma(3);
    ma.Add(10.0);
    ma.Add(20.0);
    EXPECT_NEAR(ma.Average(), 15.0, std::numeric_limits<double>::epsilon());

    ma.EnsureSize(5);
    EXPECT_EQ(ma.Capacity(), 5);
    EXPECT_NEAR(ma.Average(), 0.0, std::numeric_limits<double>::epsilon());
}

TEST(AccurateMovingAverageTest, EnsureAndResize)
{
    AccurateMovingAverage ama(3);
    ama.Add(10.0);
    ama.Add(20.0);
    EXPECT_NEAR(ama.Average(), 15.0, std::numeric_limits<double>::epsilon());

    ama.EnsureSize(5);
    EXPECT_EQ(ama.Capacity(), 5);
    EXPECT_NEAR(ama.Average(), 0.0, std::numeric_limits<double>::epsilon());
}
