#include <gtest/gtest.h>

#include <format>

#include "Core.h"

using namespace Voxium::Core;

TEST(RectangleTest, Constructors)
{
    Rectangle r1(1.0, 2.0, 3.0, 4.0);
    EXPECT_DOUBLE_EQ(r1.Left, 1.0);
    EXPECT_DOUBLE_EQ(r1.Top, 2.0);
    EXPECT_DOUBLE_EQ(r1.Width, 3.0);
    EXPECT_DOUBLE_EQ(r1.Height, 4.0);

    Vector    tl(5.0, 6.0);
    Size      s(7.0, 8.0);
    Rectangle r2(tl, s);
    EXPECT_DOUBLE_EQ(r2.Left, 5.0);
    EXPECT_DOUBLE_EQ(r2.Top, 6.0);
    EXPECT_DOUBLE_EQ(r2.Width, 7.0);
    EXPECT_DOUBLE_EQ(r2.Height, 8.0);

    Rectangle r3;
    EXPECT_DOUBLE_EQ(r3.Left, 0.0);
    EXPECT_DOUBLE_EQ(r3.Top, 0.0);
    EXPECT_DOUBLE_EQ(r3.Width, 0.0);
    EXPECT_DOUBLE_EQ(r3.Height, 0.0);
}

TEST(RectangleTest, IsEmpty)
{
    Rectangle r1(0, 0, 0, 10);
    EXPECT_TRUE(r1.IsEmpty());

    Rectangle r2(0, 0, 10, 0);
    EXPECT_TRUE(r2.IsEmpty());

    Rectangle r3(0, 0, 10, 10);
    EXPECT_FALSE(r3.IsEmpty());
}

TEST(RectangleTest, BottomAndSetBottom)
{
    Rectangle r(1, 2, 10, 20);
    EXPECT_DOUBLE_EQ(r.Bottom(), 22);
    r.SetBottom(30);
    EXPECT_DOUBLE_EQ(r.Height, 28);
}

TEST(RectangleTest, RightAndSetRight)
{
    Rectangle r(1, 2, 10, 20);
    EXPECT_DOUBLE_EQ(r.Right(), 11);
    r.SetRight(20);
    EXPECT_DOUBLE_EQ(r.Width, 19);
}

TEST(RectangleTest, CenterAndCenterSnapped)
{
    Rectangle r(0, 0, 10, 10);
    Vector    center = r.Center();
    EXPECT_DOUBLE_EQ(center.X, 5.0);
    EXPECT_DOUBLE_EQ(center.Y, 5.0);

    Vector centerSnapped = r.CenterSnapped();
    EXPECT_DOUBLE_EQ(centerSnapped.X, std::floor(5.0));
    EXPECT_DOUBLE_EQ(centerSnapped.Y, std::floor(5.0));
}

TEST(RectangleTest, Clip)
{
    Rectangle inner(2, 2, 5, 5);
    Rectangle outer(3, 3, 4, 4);
    Rectangle clipped = inner.Clip(outer);
    EXPECT_DOUBLE_EQ(clipped.Left, 3);
    EXPECT_DOUBLE_EQ(clipped.Top, 3);
    EXPECT_DOUBLE_EQ(clipped.Width, 7 - 3);
    EXPECT_DOUBLE_EQ(clipped.Height, 7 - 3);

    Rectangle r1(0, 0, 2, 2);
    Rectangle r2(3, 3, 2, 2);
    Rectangle clipped2 = r1.Clip(r2);
    EXPECT_DOUBLE_EQ(clipped2.Width, -1);
    EXPECT_DOUBLE_EQ(clipped2.Height, -1);
}

TEST(RectangleTest, ApplyInsets)
{
    Rectangle r(0, 0, 10, 10);
    Rectangle inset1 = r.ApplyInsets(1);
    EXPECT_DOUBLE_EQ(inset1.Left, 1);
    EXPECT_DOUBLE_EQ(inset1.Top, 1);
    EXPECT_DOUBLE_EQ(inset1.Width, 10 - 2);
    EXPECT_DOUBLE_EQ(inset1.Height, 10 - 2);

    Rectangle inset2 = r.ApplyInsets(1, 2, 3, 4);
    EXPECT_DOUBLE_EQ(inset2.Left, 0 + 1);
    EXPECT_DOUBLE_EQ(inset2.Top, 0 + 2);
    EXPECT_DOUBLE_EQ(inset2.Width, 10 - 1 - 3);
    EXPECT_DOUBLE_EQ(inset2.Height, 10 - 2 - 4);
}

TEST(RectangleTest, ApplyOutsets)
{
    Rectangle r(0, 0, 10, 10);
    Rectangle outset1 = r.ApplyOutsets(1);
    EXPECT_DOUBLE_EQ(outset1.Left, -1);
    EXPECT_DOUBLE_EQ(outset1.Top, -1);
    EXPECT_DOUBLE_EQ(outset1.Width, 10 + 2);
    EXPECT_DOUBLE_EQ(outset1.Height, 10 + 2);

    Rectangle outset2 = r.ApplyOutsets(1, 2, 3, 4);
    EXPECT_DOUBLE_EQ(outset2.Left, 0 - 1);
    EXPECT_DOUBLE_EQ(outset2.Top, 0 - 2);
    EXPECT_DOUBLE_EQ(outset2.Width, 10 + 1 + 3);
    EXPECT_DOUBLE_EQ(outset2.Height, 10 + 2 + 4);
}

TEST(RectangleTest, Inflate)
{
    Rectangle r(10, 10, 20, 20);
    Rectangle inflated = r.Inflate(2, 3);
    EXPECT_DOUBLE_EQ(inflated.Left, 10 - 2);
    EXPECT_DOUBLE_EQ(inflated.Top, 10 - 3);
    EXPECT_DOUBLE_EQ(inflated.Width, 20 + 2 * 2);
    EXPECT_DOUBLE_EQ(inflated.Height, 20 + 2 * 3);
}

TEST(RectangleTest, Contains)
{
    Rectangle r(0, 0, 10, 10);
    Vector    inside(5, 5);
    Vector    onEdge(10, 10);
    Vector    outside(11, 11);
    EXPECT_TRUE(r.Contains(inside));
    EXPECT_FALSE(r.Contains(onEdge));
    EXPECT_FALSE(r.Contains(outside));
}

TEST(RectangleTest, TopLeftTopRightBottomLeftBottomRight)
{
    Rectangle r(1, 2, 3, 4);
    Vector    topLeft = r.TopLeft();
    EXPECT_DOUBLE_EQ(topLeft.X, 1);
    EXPECT_DOUBLE_EQ(topLeft.Y, 2);

    Vector topRight = r.TopRight();
    EXPECT_DOUBLE_EQ(topRight.X, r.Right()); // 1 + 3 = 4
    EXPECT_DOUBLE_EQ(topRight.Y, 2);

    Vector bottomLeft = r.BottomLeft();
    EXPECT_DOUBLE_EQ(bottomLeft.X, 1);
    EXPECT_DOUBLE_EQ(bottomLeft.Y, r.Bottom()); // 2 + 4 = 6

    Vector bottomRight = r.BottomRight();
    EXPECT_DOUBLE_EQ(bottomRight.X, r.Right());
    EXPECT_DOUBLE_EQ(bottomRight.Y, r.Bottom());
}

TEST(RectangleTest, SetTopLeftSetTopRightSetBottomRight)
{
    Rectangle r(1, 2, 3, 4);
    r.SetTopLeft(Vector(10, 20));
    EXPECT_DOUBLE_EQ(r.Left, 10);
    EXPECT_DOUBLE_EQ(r.Top, 20);

    r.SetTopRight(Vector(30, 40));
    EXPECT_DOUBLE_EQ(r.Width, 20);
    EXPECT_DOUBLE_EQ(r.Top, 40);

    r.SetBottomRight(Vector(50, 80));
    EXPECT_DOUBLE_EQ(r.Width, 40);
    EXPECT_DOUBLE_EQ(r.Height, 40);
}

TEST(RectangleTest, SizeAndSetSize)
{
    Rectangle r(5, 5, 10, 10);
    Size      s = r.Size();
    EXPECT_DOUBLE_EQ(s.Width, 10);
    EXPECT_DOUBLE_EQ(s.Height, 10);

    Size newSize(20, 30);
    r.SetSize(newSize);
    EXPECT_DOUBLE_EQ(r.Width, 20);
    EXPECT_DOUBLE_EQ(r.Height, 30);
}

TEST(RectangleTest, ToString)
{
    Rectangle   r(1.234, 5.678, 9.1011, 2.3456);
    std::string s        = r.ToString();
    std::string expected = std::format("[{:.2f},{:.2f} {:.2f}x{:.2f}]", r.Left, r.Top, r.Width, r.Height);
    EXPECT_EQ(s, expected);
}

TEST(RectangleTest, ScaleOperators)
{
    Rectangle r(1, 2, 3, 4);
    Rectangle scaled = r * 2;
    EXPECT_DOUBLE_EQ(scaled.Left, 2);
    EXPECT_DOUBLE_EQ(scaled.Top, 4);
    EXPECT_DOUBLE_EQ(scaled.Width, 6);
    EXPECT_DOUBLE_EQ(scaled.Height, 8);

    Rectangle divided = r / 2;
    EXPECT_DOUBLE_EQ(divided.Left, 1 / 2.0);
    EXPECT_DOUBLE_EQ(divided.Top, 2 / 2.0);
    EXPECT_DOUBLE_EQ(divided.Width, 3 / 2.0);
    EXPECT_DOUBLE_EQ(divided.Height, 4 / 2.0);
}

TEST(RectangleTest, ApplyGravityFill)
{
    Rectangle r(0, 0, 200, 200);
    Size      content(50, 50);
    Rectangle result = r.ApplyGravity(content, Gravity::Fill);
    EXPECT_EQ(result, r);
}

TEST(RectangleTest, ApplyGravityRightBottom)
{
    Rectangle r(0, 0, 200, 200);
    Size      content(50, 50);
    Gravity   align  = Gravity::Right | Gravity::Bottom;
    Rectangle result = r.ApplyGravity(content, align);
    EXPECT_DOUBLE_EQ(result.Left, r.Right() - content.Width);
    EXPECT_DOUBLE_EQ(result.Top, r.Bottom() - content.Height);
    EXPECT_DOUBLE_EQ(result.Width, content.Width);
    EXPECT_DOUBLE_EQ(result.Height, content.Height);
}

TEST(RectangleTest, ApplyGravityCenter)
{
    Rectangle r(0, 0, 200, 200);
    Size      content(50, 50);
    Gravity   align  = Gravity::CenterHorizontal | Gravity::CenterVertical;
    Rectangle result = r.ApplyGravity(content, align);
    EXPECT_DOUBLE_EQ(result.Left, (r.Left + r.Right() - content.Width) / 2.0);
    EXPECT_DOUBLE_EQ(result.Top, (r.Top + r.Bottom() - content.Height) / 2.0);
    EXPECT_DOUBLE_EQ(result.Width, content.Width);
    EXPECT_DOUBLE_EQ(result.Height, content.Height);
}
