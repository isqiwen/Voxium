#pragma once

#include <cmath>
#include <limits>
#include <string>
#include <sstream>

#include "Core/Base/Hash.h"
#include "Vector3.h"
#include "Vector3F.h"

namespace Voxium::Core {

#pragma pack(push, 1)
    struct Int3 {
        int X;
        int Y;
        int Z;

        static constexpr int INVALID_X;
        static constexpr Int3 Zero;
        static constexpr Int3 Max;
        static constexpr Int3 Invalid;
        static constexpr Int3 FullInvalid;
        static constexpr Vector3 InvalidV;

        constexpr Int3(int x, int y, int z) : X(x), Y(y), Z(z) {}

        constexpr Int3(double x, double y, double z)
            : X(static_cast<int>(x)), Y(static_cast<int>(y)), Z(static_cast<int>(z)) {}

        constexpr Int3(const Vector3& v) : X(v.X), Y(v.Y), Z(v.Z) {}

        constexpr Int3(const Vector3F& v) : X(v.X), Y(v.Y), Z(v.Z) {}

        constexpr void Reset(double x, double y, double z) {
            X = static_cast<int>(x);
            Y = static_cast<int>(y);
            Z = static_cast<int>(z);
        }

        constexpr void Reset(int x, int y, int z) {
            X = x;
            Y = y;
            Z = z;
        }

        double Magnitude() const {
            return std::sqrt(static_cast<double>(X * X + Y * Y + Z * Z));
        }

        double MagnitudeXZ() const {
            return std::sqrt(static_cast<double>(X * X + Z * Z));
        }

        constexpr bool operator==(const Int3& other) const {
            return X == other.X && Y == other.Y && Z == other.Z;
        }
        constexpr bool operator!=(const Int3& other) const {
            return !(*this == other);
        }

        constexpr operator Vector3() const {
            return Vector3(X, Y, Z);
        }

        constexpr operator Vector3F() const {
            return Vector3F(X, Y, Z);
        }

        constexpr std::size_t GetHashCode() const
        {
            size_t hash = 0;
            HashCombine(hash, X, Y, Z);
            return hash;
        }

        std::string ToString() const { return std::format("({},{},{})", X, Y, Z) };
    };
#pragma pack(pop)

    inline constexpr int Int3::INVALID_X = -1;
    inline constexpr Int3 Int3::Zero = Int3(0, 0, 0);
    inline constexpr Int3 Int3::Max = Int3(std::numeric_limits<int>::max(), std::numeric_limits<int>::max(), std::numeric_limits<int>::max());
    inline constexpr Int3 Int3::Invalid = Int3(Int3::INVALID_X, 0, 0);
    inline constexpr Int3 Int3::FullInvalid = Int3(Int3::INVALID_X, Int3::INVALID_X, Int3::INVALID_X);;
    inline constexpr Vector3 Int3::InvalidV = Int3(Int3::INVALID_X, 0, 0);

    constexpr Vector3 operator+(const Int3& v, const Vector3& i) {
        return Vector3(v.X + i.X, v.Y + i.Y, v.Z + i.Z);
    }

    constexpr Vector3 operator-(const Int3& v, const Vector3& i) {
        return Vector3(v.X - i.X, v.Y - i.Y, v.Z - i.Z);
    }

    constexpr Int3 operator+(const Int3& a, const Int3& b) {
        return Int3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
    }

    constexpr Int3 operator-(const Int3& a, const Int3& b) {
        return Int3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
    }

    constexpr Int3 operator*(const Int3& v, int i) {
        return Int3(v.X * i, v.Y * i, v.Z * i);
    }

    constexpr Int3 operator/(const Int3& v, int i) {
        return Int3(v.X / i, v.Y / i, v.Z / i);
    }

    constexpr bool operator>(const Vector3& v, const Int3& i) {
        return v.X > i.X && v.Y > i.Y && v.Z > i.Z;
    }

    constexpr bool operator<(const Vector3& v, const Int3& i) {
        return v.X < i.X && v.Y < i.Y && v.Z < i.Z;
    }

    constexpr bool operator>=(const Vector3& v, const Int3& i) {
        return v.X >= i.X && v.Y >= i.Y && v.Z >= i.Z;
    }

    constexpr bool operator<=(const Vector3& v, const Int3& i) {
        return v.X <= i.X && v.Y <= i.Y && v.Z <= i.Z;
    }

    constexpr bool Valid(const Int3& i) {
        return i.X != Int3::INVALID_X;
    }

    constexpr bool Valid(const Vector3& v) {
        return v.X != Int3::INVALID_X;
    }
}
