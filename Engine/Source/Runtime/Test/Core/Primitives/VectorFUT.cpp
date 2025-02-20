#include <gtest/gtest.h>

#include "Core/Primitives/VectorF.h"

using namespace Voxium::Core;

TEST(VectorFTest, ConstructorsAndZero)
{
    VectorF v1;
    EXPECT_FLOAT_EQ(v1.X, 0.0);
    EXPECT_FLOAT_EQ(v1.Y, 0.0);

    VectorF v2(3.0, 4.0);
    EXPECT_FLOAT_EQ(v2.X, 3.0);
    EXPECT_FLOAT_EQ(v2.Y, 4.0);

    VectorF v3(5.0);
    EXPECT_FLOAT_EQ(v3.X, 5.0);
    EXPECT_FLOAT_EQ(v3.Y, 5.0);

    EXPECT_TRUE(v1 == VectorF::Zero);
}

TEST(VectorFTest, MagnitudeAndNormalization)
{
    VectorF v(3.0, 4.0);
    EXPECT_FLOAT_EQ(v.Magnitude(), 5.0);

    VectorF norm = v.Normalized();
    EXPECT_NEAR(norm.X, 0.6, std::numeric_limits<float>::epsilon());
    EXPECT_NEAR(norm.Y, 0.8, std::numeric_limits<float>::epsilon());

    VectorF vCopy = v;
    vCopy.Normalize();
    EXPECT_NEAR(vCopy.X, 0.6, std::numeric_limits<float>::epsilon());
    EXPECT_NEAR(vCopy.Y, 0.8, std::numeric_limits<float>::epsilon());
}

TEST(VectorFTest, ArithmeticOperators)
{
    VectorF a(1.0, 2.0);
    VectorF b(3.0, 4.0);

    VectorF sum = a + b;
    EXPECT_FLOAT_EQ(sum.X, 4.0);
    EXPECT_FLOAT_EQ(sum.Y, 6.0);

    VectorF diff = b - a;
    EXPECT_FLOAT_EQ(diff.X, 2.0);
    EXPECT_FLOAT_EQ(diff.Y, 2.0);

    VectorF neg = -a;
    EXPECT_FLOAT_EQ(neg.X, -1.0);
    EXPECT_FLOAT_EQ(neg.Y, -2.0);

    VectorF scaled = a * 2.0;
    EXPECT_FLOAT_EQ(scaled.X, 2.0);
    EXPECT_FLOAT_EQ(scaled.Y, 4.0);

    VectorF divided = b / 2.0;
    EXPECT_FLOAT_EQ(divided.X, 1.5);
    EXPECT_FLOAT_EQ(divided.Y, 2.0);
}

TEST(VectorFTest, DotAndCrossProduct)
{
    VectorF a(1.0, 2.0);
    VectorF b(3.0, 4.0);
    float   dotMember = a.DotProduct(b);
    EXPECT_FLOAT_EQ(dotMember, 1.0 * 3.0 + 2.0 * 4.0); // 11.0

    VectorF pointA(0, 0);
    VectorF pointB(1, 1);
    VectorF pointC(2, 2);
    float   dotStatic = VectorF::DotProduct(pointA, pointB, pointC);
    EXPECT_FLOAT_EQ(dotStatic, 2.0);

    VectorF A(0, 0);
    VectorF B(1, 0);
    VectorF C(0, 1);
    float   cross = VectorF::CrossProduct(A, B, C);
    EXPECT_FLOAT_EQ(cross, 1.0);
}

TEST(VectorFTest, Distance)
{
    VectorF a(1.0, 2.0);
    VectorF b(4.0, 6.0);
    float   dist = VectorF::Distance(a, b);
    EXPECT_FLOAT_EQ(dist, 5.0);
}

TEST(VectorFTest, Rotate)
{
    VectorF point(1, 0);
    VectorF center(0, 0);
    float   angle   = 3.14159265358979323846 / 2;
    VectorF rotated = VectorF::Rotate(point, center, angle);
    EXPECT_NEAR(rotated.X, 0.0, std::numeric_limits<float>::epsilon());
    EXPECT_NEAR(rotated.Y, 1.0, std::numeric_limits<float>::epsilon());
}

TEST(VectorFTest, NormalAndScale)
{
    VectorF v(3.0, 4.0);
    VectorF normal = v.Normal();
    EXPECT_FLOAT_EQ(normal.X, 4.0);
    EXPECT_FLOAT_EQ(normal.Y, -3.0);

    VectorF scaled = v.Scale(2.0);
    EXPECT_FLOAT_EQ(scaled.X, 6.0);
    EXPECT_FLOAT_EQ(scaled.Y, 8.0);
}

TEST(VectorFTest, ToString)
{
    VectorF     v(3.14159, 2.71828);
    std::string str      = v.ToString();
    std::string expected = "[3.14,2.72]";
    EXPECT_EQ(str, expected);
}
