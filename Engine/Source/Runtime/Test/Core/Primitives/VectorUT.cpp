#include <gtest/gtest.h>

#include "Core/Primitives/Vector.h"

using namespace Voxium::Core;

TEST(VectorTest, ConstructorsAndZero)
{
    Vector v1;
    EXPECT_DOUBLE_EQ(v1.X, 0.0);
    EXPECT_DOUBLE_EQ(v1.Y, 0.0);

    Vector v2(3.0, 4.0);
    EXPECT_DOUBLE_EQ(v2.X, 3.0);
    EXPECT_DOUBLE_EQ(v2.Y, 4.0);

    Vector v3(5.0);
    EXPECT_DOUBLE_EQ(v3.X, 5.0);
    EXPECT_DOUBLE_EQ(v3.Y, 5.0);

    EXPECT_TRUE(v1 == Vector::Zero);
}

TEST(VectorTest, MagnitudeAndNormalization)
{
    Vector v(3.0, 4.0);
    EXPECT_DOUBLE_EQ(v.Magnitude(), 5.0);

    Vector norm = v.Normalized();
    EXPECT_NEAR(norm.X, 0.6, std::numeric_limits<double>::epsilon());
    EXPECT_NEAR(norm.Y, 0.8, std::numeric_limits<double>::epsilon());

    Vector vCopy = v;
    vCopy.Normalize();
    EXPECT_NEAR(vCopy.X, 0.6, std::numeric_limits<double>::epsilon());
    EXPECT_NEAR(vCopy.Y, 0.8, std::numeric_limits<double>::epsilon());
}

TEST(VectorTest, ArithmeticOperators)
{
    Vector a(1.0, 2.0);
    Vector b(3.0, 4.0);

    Vector sum = a + b;
    EXPECT_DOUBLE_EQ(sum.X, 4.0);
    EXPECT_DOUBLE_EQ(sum.Y, 6.0);

    Vector diff = b - a;
    EXPECT_DOUBLE_EQ(diff.X, 2.0);
    EXPECT_DOUBLE_EQ(diff.Y, 2.0);

    Vector neg = -a;
    EXPECT_DOUBLE_EQ(neg.X, -1.0);
    EXPECT_DOUBLE_EQ(neg.Y, -2.0);

    Vector scaled = a * 2.0;
    EXPECT_DOUBLE_EQ(scaled.X, 2.0);
    EXPECT_DOUBLE_EQ(scaled.Y, 4.0);

    Vector divided = b / 2.0;
    EXPECT_DOUBLE_EQ(divided.X, 1.5);
    EXPECT_DOUBLE_EQ(divided.Y, 2.0);
}

TEST(VectorTest, DotAndCrossProduct)
{
    Vector a(1.0, 2.0);
    Vector b(3.0, 4.0);
    double dotMember = a.DotProduct(b);
    EXPECT_DOUBLE_EQ(dotMember, 1.0 * 3.0 + 2.0 * 4.0); // 11.0

    Vector pointA(0, 0);
    Vector pointB(1, 1);
    Vector pointC(2, 2);
    double dotStatic = Vector::DotProduct(pointA, pointB, pointC);
    EXPECT_DOUBLE_EQ(dotStatic, 2.0);

    Vector A(0, 0);
    Vector B(1, 0);
    Vector C(0, 1);
    double cross = Vector::CrossProduct(A, B, C);
    EXPECT_DOUBLE_EQ(cross, 1.0);
}

TEST(VectorTest, Distance)
{
    Vector a(1.0, 2.0);
    Vector b(4.0, 6.0);
    double dist = Vector::Distance(a, b);
    EXPECT_DOUBLE_EQ(dist, 5.0);
}

TEST(VectorTest, Rotate)
{
    Vector point(1, 0);
    Vector center(0, 0);
    double angle   = 3.14159265358979323846 / 2;
    Vector rotated = Vector::Rotate(point, center, angle);
    EXPECT_NEAR(rotated.X, 0.0, std::numeric_limits<double>::epsilon());
    EXPECT_NEAR(rotated.Y, 1.0, std::numeric_limits<double>::epsilon());
}

TEST(VectorTest, NormalAndScale)
{
    Vector v(3.0, 4.0);
    Vector normal = v.Normal();
    EXPECT_DOUBLE_EQ(normal.X, 4.0);
    EXPECT_DOUBLE_EQ(normal.Y, -3.0);

    Vector scaled = v.Scale(2.0);
    EXPECT_DOUBLE_EQ(scaled.X, 6.0);
    EXPECT_DOUBLE_EQ(scaled.Y, 8.0);
}

TEST(VectorTest, ToString)
{
    Vector      v(3.14159, 2.71828);
    std::string str      = v.ToString();
    std::string expected = "[3.14,2.72]";
    EXPECT_EQ(str, expected);
}
