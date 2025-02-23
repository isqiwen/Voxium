#pragma once

#include <string>

#include "CoreMacros.h"

namespace Voxium::Core
{
    struct Size;
    struct VectorF;

#pragma pack(push, 1)
    struct CORE_API Vector
    {
        double              X, Y;
        static const Vector Zero;

        constexpr Vector(double x, double y) : X(x), Y(y) {}
        constexpr Vector(double value) : X(value), Y(value) {}
        constexpr Vector() : X(0), Y(0) {}

        // ===================================================================
        // Public Member Function
        // ===================================================================

        bool             operator==(const Vector& other) const;
        bool             operator!=(const Vector& other) const;
        constexpr Vector operator+(const Vector& other) const { return Vector(X + other.X, Y + other.Y); }
        constexpr Vector operator-(const Vector& other) const { return Vector(X - other.X, Y - other.Y); }
        constexpr Vector operator-() const { return Vector(-X, -Y); }

        bool             IsEqual(const Vector& other) const;
        constexpr double DotProduct(const Vector& other) const { return X * other.X + Y * other.Y; }
        constexpr double CrossProduct(const Vector& other) const { return X * other.Y - Y * other.X; }
        constexpr Vector Normal() const { return Vector(Y, -X); }
        constexpr Vector Scale(double scale) const { return Vector(X * scale, Y * scale); }
        double           Magnitude() const;
        void             Normalize();
        Vector           Normalized() const;
        std::size_t      GetHashCode() const;
        VectorF          ToFloat() const;
        Size             ToSize() const;
        std::string      ToString() const;

        // ===================================================================
        // Static Member Function
        // ===================================================================

        static double Distance(const Vector& a, const Vector& b);
        static Vector Rotate(const Vector& point, const Vector& center, double angle);

        constexpr static double DotProduct(const Vector& pointA, const Vector& pointB, const Vector& pointC)
        {
            Vector AB = pointB - pointA;
            Vector BC = pointC - pointB;
            return AB.X * BC.X + AB.Y * BC.Y;
        }

        constexpr static double CrossProduct(const Vector& pointA, const Vector& pointB, const Vector& pointC)
        {
            Vector AB = pointB - pointA;
            Vector AC = pointC - pointA;
            return AB.X * AC.Y - AB.Y * AC.X;
        }
    };
#pragma pack(pop)

    constexpr Vector operator*(const Vector& a, double scale) { return Vector(a.X * scale, a.Y * scale); }
    constexpr Vector operator/(const Vector& a, double scale) { return Vector(a.X / scale, a.Y / scale); }

} // namespace Voxium::Core
