#pragma once

#include <string>

#include "CoreMacros.h"

#include "Math/VectorF.h"

namespace Voxium::Core
{

    struct Int3;
    struct Vector3;
    struct Vector4F;

#pragma pack(push, 1)
    struct CORE_API Vector3F
    {
        float                 X, Y, Z;
        static const Vector3F Zero;

        constexpr Vector3F(float x) : X(x), Y(x), Z(x) {}
        constexpr Vector3F(float x, float y, float z) : X(x), Y(y), Z(z) {}
        Vector3F(const Int3& v);
        Vector3F(const Vector4F& v);

        bool               operator==(const Vector3F& other);
        bool               operator!=(const Vector3F& other);
        constexpr Vector3F operator+(const Vector3F& other) const { return Vector3F(X + other.X, Y + other.Y, Z + other.Z); }
        constexpr Vector3F operator-(const Vector3F& other) const { return Vector3F(X - other.X, Y - other.Y, Z - other.Z); }
        constexpr Vector3F operator*(const Vector3F& other) const { return Vector3F(X * other.X, Y * other.Y, Z * other.Z); }
        constexpr Vector3F operator/(const Vector3F& other) const { return Vector3F(X / other.X, Y / other.Y, Z / other.Z); }
        constexpr Vector3F operator-() { return Vector3F(-X, -Y, -Z); }

        bool               IsEqual(const Vector3F& other) const;
        constexpr VectorF  XY() const { return VectorF(X, Y); }
        constexpr VectorF  XZ() const { return VectorF(X, Z); }
        constexpr Vector3F XOZ() const { return Vector3F(X, 0.0f, Z); }
        Vector4F           XYZW() const;

        void        Normalize();
        float       Magnitude() const;
        Vector3F    Normalized() const;
        std::size_t GetHashCode() const;
        Vector3     ToVector3() const;
        std::string ToString() const;

        constexpr static float    DotProduct(const Vector3F& a, const Vector3F& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
        constexpr static Vector3F CrossProduct(const Vector3F& a, const Vector3F& b) { return Vector3F(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
        static Vector3F           FromPitchYaw(float pitch, float yaw);
    };
#pragma pack(pop)

    constexpr Vector3F operator*(const Vector3F& a, float b) { return Vector3F(a.X * b, a.Y * b, a.Z * b); }
    constexpr Vector3F operator/(const Vector3F& a, float b) { return Vector3F(a.X / b, a.Y / b, a.Z / b); }

    struct Matrix4;
    struct Matrix4F;

    CORE_API Vector3F operator*(const Vector3F& a, const Matrix4& b);
    CORE_API Vector3F operator*(const Vector3F& a, const Matrix4F& b);

} // namespace Voxium::Core
