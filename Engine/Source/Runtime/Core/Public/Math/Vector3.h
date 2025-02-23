#pragma once

#include <string>

#include "CoreMacros.h"

#include "Math/Vector.h"

namespace Voxium::Core
{

    struct Int3;
    struct Vector3F;
    struct Vector4;

#pragma pack(push, 1)
    struct CORE_API Vector3
    {
        double               X, Y, Z;
        static const Vector3 Zero;

        constexpr Vector3(double x) : X(x), Y(x), Z(x) {}
        constexpr Vector3(double x, double y, double z) : X(x), Y(y), Z(z) {}
        Vector3(const Int3& v);
        Vector3(const Vector4& v);

        bool              operator==(const Vector3& other);
        bool              operator!=(const Vector3& other);
        constexpr Vector3 operator+(const Vector3& other) const { return Vector3(X + other.X, Y + other.Y, Z + other.Z); }
        constexpr Vector3 operator-(const Vector3& other) const { return Vector3(X - other.X, Y - other.Y, Z - other.Z); }
        constexpr Vector3 operator*(const Vector3& other) const { return Vector3(X * other.X, Y * other.Y, Z * other.Z); }
        constexpr Vector3 operator/(const Vector3& other) const { return Vector3(X / other.X, Y / other.Y, Z / other.Z); }
        constexpr Vector3 operator-() { return Vector3(-X, -Y, -Z); }

        bool              IsEqual(const Vector3& other) const;
        constexpr Vector  XY() const { return Vector(X, Y); }
        constexpr Vector  XZ() const { return Vector(X, Z); }
        constexpr Vector3 XOZ() const { return Vector3(X, 0.0, Z); }
        Vector4           XYZW() const;

        void        Normalize();
        double      Magnitude() const;
        Vector3     Normalized() const;
        std::size_t GetHashCode() const;
        Vector3F    ToVector3F() const;
        std::string ToString() const;

        constexpr static double  DotProduct(const Vector3& a, const Vector3& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
        constexpr static Vector3 CrossProduct(const Vector3& a, const Vector3& b) { return Vector3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
        static Vector3           FromPitchYaw(double pitch, double yaw);
    };
#pragma pack(pop)

    constexpr Vector3 operator*(const Vector3& a, double b) { return Vector3(a.X * b, a.Y * b, a.Z * b); }
    constexpr Vector3 operator/(const Vector3& a, double b) { return Vector3(a.X / b, a.Y / b, a.Z / b); }

    struct Matrix4;
    struct Matrix4F;

    CORE_API Vector3 operator*(const Vector3& a, const Matrix4& b);
    CORE_API Vector3 operator*(const Vector3& a, const Matrix4F& b);

} // namespace Voxium::Core
