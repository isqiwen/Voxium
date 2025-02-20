#pragma once

#include <cstddef>
#include <cstdint>
#include <format>
#include <functional>
#include <stdexcept>
#include <string>

#include "Core/Base/Hash.h"
#include "Vector.h"

namespace Voxium::Core
{

#pragma pack(push, 1)
    struct Int2
    {
        int X;
        int Y;

        static const int  INVALID_X;
        static const Int2 Zero;
        static const Int2 Invalid;

        static constexpr bool Valid(const Int2& i) { return i.X != INVALID_X; }

        constexpr Int2(int X, int Y) : X(X), Y(Y) {}

        constexpr Int2(double X, double Y) : X(static_cast<int>(X)), Y(static_cast<int>(Y)) {}

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

        constexpr Int2 operator+(const Int2& i) const { return Int2(X + i.X, Y + i.Y); }
        constexpr Int2 operator-(const Int2& i) const { return Int2(X - i.X, Y - i.Y); }
        constexpr Int2 operator*(int i) const { return Int2(X * i, Y * i); }
        constexpr Int2 operator/(int i) const { return Int2(X / i, Y / i); }

        constexpr bool operator==(const Int2& i) const { return X == i.X && Y == i.Y; }
        constexpr bool operator!=(const Int2& i) const { return !(*this == i); }

        constexpr operator Vector() const { return Vector(static_cast<double>(X), static_cast<double>(Y)); }

        constexpr std::size_t GetHashCode() const
        {
            size_t hash = 0;
            HashCombine(hash, X, Y);
            return hash;
        }

        std::string ToString() const { return std::format("({},{})", X, Y); }
    };
#pragma pack(pop)

    inline const int  Int2::INVALID_X = -1;
    inline const Int2 Int2::Zero      = Int2(0, 0);
    inline const Int2 Int2::Invalid   = Int2(INVALID_X, 0);

} // namespace Voxium::Core
