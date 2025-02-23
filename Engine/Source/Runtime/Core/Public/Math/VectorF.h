#pragma once

#include <string>

#include "CoreMacros.h"

namespace Voxium::Core
{
    struct Size;
    struct Vector;

#pragma pack(push, 1)
    struct CORE_API VectorF
    {
        float                X, Y;
        static const VectorF Zero;

        constexpr VectorF(float x, float y) : X(x), Y(y) {}
        constexpr VectorF(float value) : X(value), Y(value) {}
        constexpr VectorF() : X(0), Y(0) {}

        // ===================================================================
        // Public Member Function
        // ===================================================================

        bool              operator==(const VectorF& other) const;
        bool              operator!=(const VectorF& other) const;
        constexpr VectorF operator+(const VectorF& other) const { return VectorF(X + other.X, Y + other.Y); }
        constexpr VectorF operator-(const VectorF& other) const { return VectorF(X - other.X, Y - other.Y); }
        constexpr VectorF operator-() const { return VectorF(-X, -Y); }

        bool              IsEqual(const VectorF& other) const;
        constexpr float   DotProduct(const VectorF& other) const { return X * other.X + Y * other.Y; }
        constexpr float   CrossProduct(const VectorF& other) const { return X * other.Y - Y * other.X; }
        constexpr VectorF Normal() const { return VectorF(Y, -X); }
        constexpr VectorF Scale(float scale) const { return VectorF(X * scale, Y * scale); }
        float             Magnitude() const;
        void              Normalize();
        VectorF           Normalized() const;
        std::size_t       GetHashCode() const;
        Vector            ToDouble() const;
        Size              ToSize() const;
        std::string       ToString() const;

        // ===================================================================
        // Static Member Function
        // ===================================================================

        static float   Distance(const VectorF& a, const VectorF& b);
        static VectorF Rotate(const VectorF& point, const VectorF& center, float angle);

        constexpr static float DotProduct(const VectorF& pointA, const VectorF& pointB, const VectorF& pointC)
        {
            VectorF AB = pointB - pointA;
            VectorF BC = pointC - pointB;
            return AB.X * BC.X + AB.Y * BC.Y;
        }

        constexpr static float CrossProduct(const VectorF& pointA, const VectorF& pointB, const VectorF& pointC)
        {
            VectorF AB = pointB - pointA;
            VectorF AC = pointC - pointA;
            return AB.X * AC.Y - AB.Y * AC.X;
        }
    };
#pragma pack(pop)

    constexpr VectorF operator*(const VectorF& a, float scale) { return VectorF(a.X * scale, a.Y * scale); }
    constexpr VectorF operator/(const VectorF& a, float scale) { return VectorF(a.X / scale, a.Y / scale); }

} // namespace Voxium::Core
