#pragma once

#include <string>

#include "CoreMacros.h"

namespace Voxium::Core
{

    struct Vector;

#pragma pack(push, 1)
    struct CORE_API Int2
    {
        int               X, Y;
        static const int  INVALID_X;
        static const Int2 Zero;
        static const Int2 Invalid;

        constexpr Int2(int X, int Y) : X(X), Y(Y) {}
        constexpr Int2(double X, double Y) : X(static_cast<int>(X)), Y(static_cast<int>(Y)) {}

        constexpr bool operator==(const Int2& other) const { return IsEqual(other); }
        constexpr bool operator!=(const Int2& other) const { return IsEqual(other); }
        constexpr Int2 operator+(const Int2& other) const { return Int2(X + other.X, Y + other.Y); }
        constexpr Int2 operator-(const Int2& other) const { return Int2(X - other.X, Y - other.Y); }
        constexpr Int2 operator-() const { return Int2(-X, -Y); }

        constexpr bool IsEqual(const Int2& other) const { return X == other.X && Y == other.Y; }

        constexpr bool NextTo(const Int2& v) const
        {
            if (X == v.X)
            {
                return (Y == v.Y - 1 || Y == v.Y + 1);
            }

            if (Y == v.Y)
            {
                return (X == v.X - 1 || X == v.X + 1);
            }

            return false;
        }

        std::size_t GetHashCode() const;
        Vector      ToVector() const;
        std::string ToString() const;

        constexpr static bool Valid(const Int2& i) { return i.X != INVALID_X; }
    };
#pragma pack(pop)

    CORE_API constexpr Int2 operator*(const Int2& a, int i) { return Int2(a.X * i, a.Y * i); }
    CORE_API constexpr Int2 operator/(const Int2& a, int i) { return Int2(a.X / i, a.Y / i); }

} // namespace Voxium::Core
