#pragma once

#include <string>

#include "CoreMacros.h"

namespace Voxium::Core
{

    struct Vector3;
    struct Vector3F;

#pragma pack(push, 1)
    struct CORE_API Int3
    {
        int X, Y, Z;

        static const int     INVALID_X;
        static const Int3    Zero;
        static const Int3    Max;
        static const Int3    Invalid;
        static const Int3    FullInvalid;
        static const Vector3 InvalidV;

        constexpr Int3(int x, int y, int z) : X(x), Y(y), Z(z) {}
        constexpr Int3(double x, double y, double z) : X(static_cast<int>(x)), Y(static_cast<int>(y)), Z(static_cast<int>(z)) {}
        Int3(const Vector3& v);
        Int3(const Vector3F& v);

        constexpr bool operator==(const Int3& other) const { return IsEqual(other); }
        constexpr bool operator!=(const Int3& other) const { return IsEqual(other); }
        constexpr Int3 operator+(const Int3& other) const { return Int3(X + other.X, Y + other.Y, Z + other.Z); }
        constexpr Int3 operator-(const Int3& other) const { return Int3(X - other.X, Y - other.Y, Z - other.Z); }
        constexpr Int3 operator-() const { return Int3(-X, -Y, -Z); }

        constexpr bool IsEqual(const Int3& other) const { return X == other.X && Y == other.Y && Z == other.Z; }

        constexpr void Reset(double x, double y, double z)
        {
            X = static_cast<int>(x);
            Y = static_cast<int>(y);
            Z = static_cast<int>(z);
        }

        constexpr void Reset(int x, int y, int z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        double      Magnitude() const;
        double      MagnitudeXZ() const;
        std::size_t GetHashCode() const;
        Vector3     ToVector3() const;
        Vector3F    ToVector3F() const;
        std::string ToString() const;

        constexpr static bool Valid(const Int3& i) { return i.X != Int3::INVALID_X; }
        static bool           Valid(const Vector3& v);
    };
#pragma pack(pop)

} // namespace Voxium::Core
