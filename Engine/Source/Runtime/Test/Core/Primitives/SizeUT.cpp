#include <gtest/gtest.h>

#include "Core/Primitives/Size.h"

using namespace Voxium::Core;

TEST(SizeTest, ConstructorsAndEmpty)
{
    Size s1;
    EXPECT_DOUBLE_EQ(s1.Width, 0.0);
    EXPECT_DOUBLE_EQ(s1.Height, 0.0);

    Size s2(5.0, 10.0);
    EXPECT_DOUBLE_EQ(s2.Width, 5.0);
    EXPECT_DOUBLE_EQ(s2.Height, 10.0);

    Size s3(7.0);
    EXPECT_DOUBLE_EQ(s3.Width, 7.0);
    EXPECT_DOUBLE_EQ(s3.Height, 7.0);

    EXPECT_TRUE(Size::Empty == s1);
}

TEST(SizeTest, CeilingFloorRound)
{
    Size s(1.2, 2.8);
    Size ceilS = s.Ceiling();
    EXPECT_DOUBLE_EQ(ceilS.Width, std::ceil(1.2));
    EXPECT_DOUBLE_EQ(ceilS.Height, std::ceil(2.8));

    Size floorS = s.Floor();
    EXPECT_DOUBLE_EQ(floorS.Width, std::floor(1.2));
    EXPECT_DOUBLE_EQ(floorS.Height, std::floor(2.8));

    Size roundS = s.Round();
    EXPECT_DOUBLE_EQ(roundS.Width, std::round(1.2));
    EXPECT_DOUBLE_EQ(roundS.Height, std::round(2.8));
}

TEST(SizeTest, ScaleAndArithmetic)
{
    Size s(3.0, 4.0);

    Size scaled = s.Scale(2.0);
    EXPECT_DOUBLE_EQ(scaled.Width, 6.0);
    EXPECT_DOUBLE_EQ(scaled.Height, 8.0);

    Size s2(1.0, 2.0);
    Size add = s + s2;
    EXPECT_DOUBLE_EQ(add.Width, 4.0);
    EXPECT_DOUBLE_EQ(add.Height, 6.0);

    Size sub = s - s2;
    EXPECT_DOUBLE_EQ(sub.Width, 2.0);
    EXPECT_DOUBLE_EQ(sub.Height, 2.0);

    Size mul = s * 2.0;
    EXPECT_DOUBLE_EQ(mul.Width, 6.0);
    EXPECT_DOUBLE_EQ(mul.Height, 8.0);

    Size div = s / 2.0;
    EXPECT_DOUBLE_EQ(div.Width, 1.5);
    EXPECT_DOUBLE_EQ(div.Height, 2.0);

    Size neg = -s;
    EXPECT_DOUBLE_EQ(neg.Width, -3.0);
    EXPECT_DOUBLE_EQ(neg.Height, -4.0);

    EXPECT_TRUE(s == s);
    EXPECT_TRUE(s != s2);
}

TEST(SizeTest, Snap)
{
    Size s(1.3, 2.7);
    // 1.3/0.5 = 2.6, round(2.6)=3, 3*0.5 = 1.5
    // 2.7/0.5 = 5.4, round(5.4)=5, 5*0.5 = 2.5
    Size snapped = s.Snap(0.5);
    EXPECT_DOUBLE_EQ(snapped.Width, 1.5);
    EXPECT_DOUBLE_EQ(snapped.Height, 2.5);
}

TEST(SizeTest, ToString)
{
    Size        s(3.5, 4.5);
    std::string strDefault      = s.ToString();
    std::string expectedDefault = std::format("({},{})", s.Width, s.Height);
    EXPECT_EQ(strDefault, expectedDefault);

    std::string fmt               = ".2f";
    std::string strFormatted      = s.ToString(fmt);
    std::string expectedFormatted = std::format("({:.2f},{:.2f})", s.Width, s.Height);
    EXPECT_EQ(strFormatted, expectedFormatted);
}
