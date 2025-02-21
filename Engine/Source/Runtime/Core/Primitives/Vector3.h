#pragma once

#include <cmath>
#include <format>
#include <stdexcept>
#include <string>

#include "Core/Base/Hash.h"
#include "Int3.h"
#include "Vector.h"
#include "VectorF.h"
#include "Vector3F.h"
#include "Vector4.h"

namespace Voxium::Core
{
#pragma pack(push, 1)
    struct Vector3
    {
        double X;
        double Y;
        double Z;

        static constexpr Vector3 Zero;

        constexpr Vector3(double x) : X(x), Y(x), Z(x) {}

        constexpr Vector3(double x, double y, double z) : X(x), Y(y), Z(z) {}

        constexpr Vector3(const Int3& v) : X(v.X), Y(v.Y), Z(v.Z) {}

        static Vector3 FromPitchYaw(double pitch, double yaw)
        {
            double cosPitch = std::cos(pitch);
            return Vector3(cosPitch * std::sin(yaw), std::sin(pitch), cosPitch * std::cos(yaw));
        }

        constexpr Vector XY() const { return Vector(X, Y); }
        constexpr Vector XZ() const { return Vector(X, Z); }
        constexpr VectorF XZF() const { return VectorF(static_cast<float>(X), static_cast<float>(Z)); }
        constexpr Vector3F F() const
        {
            return Vector3F(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Z));
        }
        constexpr Vector3 XOZ() const { return Vector3(X, 0.0, Z); }
        constexpr Vector4 XYZW() const { return Vector4(X, Y, Z, 1.0); }

        constexpr double Magnitude() const { return std::sqrt(X * X + Y * Y + Z * Z); }

        void             Normalize()
        {
            double mag = Magnitude();
            if (mag == 0)
                throw std::runtime_error("Cannot normalize zero vector");
            double scale = 1.0 / mag;
            X *= scale;
            Y *= scale;
            Z *= scale;
        }

        constexpr Vector3 Normalized() const
        {
            double mag = Magnitude();
            if (mag == 0)
                return *this;
            double scale = 1.0 / mag;
            return Vector3(X * scale, Y * scale, Z * scale);
        }

        static constexpr double DotProduct(const Vector3& a, const Vector3& b)
        {
            return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
        }

        static constexpr Vector3 CrossProduct(const Vector3& a, const Vector3& b)
        {
            return Vector3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
        }

        constexpr bool IsEqual(const Vector3& other) const {
            constexpr double epsilon = std::numeric_limits<double>::epsilon();
            return std::abs(X - other.X) < epsilon && std::abs(Y - other.Y) < epsilon && std::abs(Z - other.Z) < epsilon;
        }

        constexpr std::size_t GetHashCode() const
        {
            size_t hash = 0;
            HashCombine(hash, X, Y, Z);
            return hash;
        }

        std::string ToString() const { return std::format("[{:.2f},{:.2f},{:.2f}]", X, Y, Z); }
    };
#pragma pack(pop)

    inline constexpr Vector3 Vector3::Zero = Vector3(0, 0, 0);

    constexpr bool    operator==(const Vector3& a, const Vector3& b) { return a.IsEqual(b); }
    constexpr bool    operator!=(const Vector3& a, const Vector3& b) { return !a.IsEqual(b); }

    constexpr Vector3 operator+(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
    }

    constexpr Vector3 operator-(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
    }

    constexpr Vector3 operator*(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.X * b.X, a.Y * b.Y, a.Z * b.Z);
    }

    constexpr Vector3 operator/(const Vector3& a, const Vector3& b)
    {
        return Vector3(a.X / b.X, a.Y / b.Y, a.Z / b.Z);
    }

    constexpr Vector3 operator*(const Vector3& a, double b) { return Vector3(a.X * b, a.Y * b, a.Z * b); }
    constexpr Vector3 operator/(const Vector3& a, double b) { return Vector3(a.X / b, a.Y / b, a.Z / b); }
    constexpr Vector3 operator-(const Vector3& a) { return Vector3(-a.X, -a.Y, -a.Z); }

    struct Matrix4;
    struct Matrix4F;

    Vector3 operator*(const Vector3& a, const Matrix4& b)
    {
        return Vector3(a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + 1.0 * b.M41,
                        a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + 1.0 * b.M42,
                        a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + 1.0 * b.M43);
    }

    Vector3 operator*(const Vector3& a, const Matrix4F& b)
    {
        return Vector3(a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + 1.0f * b.M41,
                        a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + 1.0f * b.M42,
                        a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + 1.0f * b.M43);
    }

} // namespace Voxium::Core
