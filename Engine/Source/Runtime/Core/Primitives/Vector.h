#pragma once

#include <cmath>
#include <format>
#include <limits>
#include <string>

#include "Core/Base/Hash.h"

namespace Voxium::Core
{

#pragma pack(push, 1)
    struct Vector
    {
        double X;
        double Y;

        static const Vector Zero;

        constexpr Vector(double x, double y) : X(x), Y(y) {}
        constexpr Vector(double value) : X(value), Y(value) {}
        constexpr Vector() : X(0), Y(0) {}

        static double Distance(const Vector& a, const Vector& b) { return (a - b).Magnitude(); }

        double DotProduct(const Vector& other) const { return X * other.X + Y * other.Y; }
        double Magnitude() const { return std::sqrt(X * X + Y * Y); }
        Vector Normal() const { return Vector(Y, -X); }
        Vector Scale(double scale) const { return Vector(X * scale, Y * scale); }

        void Normalize()
        {
            double mag = Magnitude();
            if (mag != 0)
            {
                X /= mag;
                Y /= mag;
            }
        }

        Vector Normalized() const
        {
            double mag = Magnitude();
            return (mag != 0) ? Vector(X / mag, Y / mag) : *this;
        }

        static Vector Rotate(const Vector& point, const Vector& center, double angle)
        {
            Vector delta      = point - center;
            double startAngle = std::atan2(delta.Y, delta.X);
            double newAngle   = startAngle + angle;
            double mag        = delta.Magnitude();
            return Vector(center.X + std::cos(newAngle) * mag, center.Y + std::sin(newAngle) * mag);
        }

        static double DotProduct(const Vector& pointA, const Vector& pointB, const Vector& pointC)
        {
            Vector AB = pointB - pointA;
            Vector BC = pointC - pointB;
            return AB.X * BC.X + AB.Y * BC.Y;
        }

        static double CrossProduct(const Vector& pointA, const Vector& pointB, const Vector& pointC)
        {
            Vector AB = pointB - pointA;
            Vector AC = pointC - pointA;
            return AB.X * AC.Y - AB.Y * AC.X;
        }

        bool IsEqual(const Vector& other) const
        {
            constexpr double epsilon = std::numeric_limits<double>::epsilon();
            return std::abs(X - other.X) < epsilon && std::abs(Y - other.Y) < epsilon;
        }

        std::string ToString() const { return std::format("[{:.2f},{:.2f}]", X, Y); }

        constexpr std::size_t GetHashCode() const
        {
            size_t hash = 0;
            HashCombine(hash, X, Y);
            return hash;
        }
    };
#pragma pack(pop)

    inline const Vector Vector::Zero = Vector(0.0, 0.0);

    bool             operator==(const Vector& a, const Vector& b) { return a.IsEqual(b); }
    bool             operator!=(const Vector& a, const Vector& b) { return !(a == b); }
    constexpr Vector operator+(const Vector& a, const Vector& b) { return Vector(a.X + b.X, a.Y + b.Y); }
    constexpr Vector operator-(const Vector& a, const Vector& b) { return Vector(a.X - b.X, a.Y - b.Y); }
    constexpr Vector operator*(const Vector& a, double scale) { return Vector(a.X * scale, a.Y * scale); }
    constexpr Vector operator/(const Vector& a, double scale) { return Vector(a.X / scale, a.Y / scale); }
    constexpr Vector operator-(const Vector& a) { return Vector(-a.X, -a.Y); }
} // namespace Voxium::Core
