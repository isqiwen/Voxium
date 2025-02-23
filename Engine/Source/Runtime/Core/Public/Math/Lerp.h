#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>
#include <functional>
#include <limits>
#include <numbers>
#include <string>

#include "Math/Int3.h"
#include "Math/MathUtility.h"
#include "Math/Size.h"
#include "Math/Vector.h"
#include "Math/Vector3.h"
#include "Math/Vector3F.h"
#include "Math/Vector4.h"
#include "Math/VectorF.h"

namespace Voxium::Core
{

    // SinLerp: returns (sin(v)+1)/2
    inline float SinLerp(float v) { return (std::sin(v) + 1.0f) / 2.0f; }

    // Mix functions for Vector4 and Vector3
    inline Vector4 Mix(const Vector4& firstVector, const Vector4& secondVector, double by)
    {
        double x = Mix(firstVector.X, secondVector.X, by);
        double y = Mix(firstVector.Y, secondVector.Y, by);
        double z = Mix(firstVector.Z, secondVector.Z, by);
        double w = Mix(firstVector.W, secondVector.W, by);
        return Vector4(x, y, z, w);
    }

    inline Vector3 Mix(const Vector3& firstVector, const Vector3& secondVector, double by)
    {
        double x = Mix(firstVector.X, secondVector.X, by);
        double y = Mix(firstVector.Y, secondVector.Y, by);
        double z = Mix(firstVector.Z, secondVector.Z, by);
        return Vector3(x, y, z);
    }

    // Lerp functions for various types
    inline Vector3 Lerp(const Vector3& firstVector, const Vector3& secondVector, double by)
    {
        double x = firstVector.X + by * (secondVector.X - firstVector.X);
        double y = firstVector.Y + by * (secondVector.Y - firstVector.Y);
        double z = firstVector.Z + by * (secondVector.Z - firstVector.Z);
        return Vector3(x, y, z);
    }

    inline Vector3F Lerp(const Vector3F& firstVector, const Vector3F& secondVector, float by)
    {
        float x = firstVector.X + by * (secondVector.X - firstVector.X);
        float y = firstVector.Y + by * (secondVector.Y - firstVector.Y);
        float z = firstVector.Z + by * (secondVector.Z - firstVector.Z);
        return Vector3F(x, y, z);
    }

    inline Vector Lerp(const Vector& firstVector, const Vector& secondVector, double by)
    {
        double x = firstVector.X + by * (secondVector.X - firstVector.X);
        double y = firstVector.Y + by * (secondVector.Y - firstVector.Y);
        return Vector(x, y);
    }

    inline VectorF Lerp(const VectorF& firstVector, const VectorF& secondVector, float by)
    {
        float x = firstVector.X + by * (secondVector.X - firstVector.X);
        float y = firstVector.Y + by * (secondVector.Y - firstVector.Y);
        return VectorF(x, y);
    }

    inline Size Lerp(const Size& a, const Size& b, double by)
    {
        double w = a.Width + by * (b.Width - a.Width);
        double h = a.Height + by * (b.Height - a.Height);
        return Size(w, h);
    }

    inline Int3 Lerp(const Int3& first, const Int3& second, double by)
    {
        double x = first.X + by * (second.X - first.X);
        double y = first.Y + by * (second.Y - first.Y);
        double z = first.Z + by * (second.Z - first.Z);
        return Int3(x, y, z);
    }

    // LerpSharp for Vector3 and double version
    inline double LerpSharp(double a, double b, double c)
    {
        if (IsNaNorInfinity(a) || IsNaNorInfinity(b))
            return 0;
        if (std::signbit(a) != std::signbit(b))
            return b;
        return a + c * (b - a);
    }

    inline Vector3 LerpSharp(const Vector3& firstVector, const Vector3& secondVector, double by)
    {
        double x = LerpSharp(firstVector.X, secondVector.X, by);
        double y = LerpSharp(firstVector.Y, secondVector.Y, by);
        double z = LerpSharp(firstVector.Z, secondVector.Z, by);
        return Vector3(x, y, z);
    }

    // Mix: returns x*(1-a) + y*a
    inline double Mix(double x, double y, double a) { return x * (1 - a) + y * a; }

    // Lerp: linear interpolation, same as Mix
    inline double Lerp(double a, double b, double c) { return a + c * (b - a); }

    // LerpByte: Interpolate between two byte values
    inline unsigned char LerpByte(unsigned char a, unsigned char b, double c) { return static_cast<unsigned char>(a + c * (b - a)); }

    // Lerp for CustomWatch (modifies d)
    inline void Lerp(const CustomWatch& a, const CustomWatch& b, double c, CustomWatch& d)
    {
        int a0 = a.Elapsed;
        int b0 = b.Elapsed;

        // Recreate the watch and reset it
        d.Recreate(a.min, a.max);
        d.Direction = a.Direction;
        d.Reset();

        // Interpolate the elapsed time
        d.Elapsed = static_cast<int>(a0 + c * (b0 - a0));
    }

    // Lerp for VEWorldSpaceBone
    inline VEWorldSpaceBone Lerp(const VEWorldSpaceBone& a, const VEWorldSpaceBone& b, float c)
    {
        Vector3 head = Lerp(a.head, b.head, c);
        Vector3 tail = Lerp(a.tail, b.tail, c);
        assert((head - tail).magnitude() < 2);
        return VEWorldSpaceBone(head, tail, a.d);
    }

    // Lerp for float
    inline float Lerp(float a, float b, float c)
    {
        if (c == 0.f)
            return a;
        else if (c == 1.f)
            return b;
        else
            return a + c * (b - a);
    }

    // LerpAngle: interpolate between two angles (radians)
    inline double LerpAngle(double a, double b, double by)
    {
        double s1 = (1 - by) * std::cos(a) + by * std::cos(b);
        double s2 = (1 - by) * std::sin(a) + by * std::sin(b);
        return std::atan2(s2, s1);
    }

    // Fmod: floating-point modulus using floor
    inline double Fmod(double v, double mod) { return v - mod * std::floor(v / mod); }

    // LerpAngle2: another angle interpolation implementation
    inline double LerpAngle2(double from, double to, double t)
    {
        double delta = std::fmod((from - to), 2 * std::numbers::pi);
        if (delta > std::numbers::pi)
            delta -= 2 * std::numbers::pi;
        double result = from - delta * t;
        if (result < 0)
            result += 2 * std::numbers::pi;
        else if (result > 2 * std::numbers::pi)
            result -= 2 * std::numbers::pi;
        return result;
    }

    // LerpSmooth for Vector3: Hermite interpolation using four control points
    inline Vector3 LerpSmooth(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3, double lerp)
    {
        double x = LerpHermite(v0.X, v1.X, v2.X, v3.X, lerp);
        double y = LerpHermite(v0.Y, v1.Y, v2.Y, v3.Y, lerp);
        double z = LerpHermite(v0.Z, v1.Z, v2.Z, v3.Z, lerp);
        return Vector3(x, y, z);
    }

    // LerpHermite: Hermite interpolation with tension and bias
    inline double LerpHermite(double y0, double y1, double y2, double y3, double mu, double tension = 0, double bias = 0)
    {
        double mu2 = mu * mu;
        double mu3 = mu2 * mu;
        double m0  = (y1 - y0) * (1 + bias) * (1 - tension) / 2.0 + (y2 - y1) * (1 - bias) * (1 - tension) / 2.0;
        double m1  = (y2 - y1) * (1 + bias) * (1 - tension) / 2.0 + (y3 - y2) * (1 - bias) * (1 - tension) / 2.0;
        double a0  = 2 * mu3 - 3 * mu2 + 1;
        double a1  = mu3 - 2 * mu2 + mu;
        double a2  = mu3 - mu2;
        double a3  = -2 * mu3 + 3 * mu2;
        return (a0 * y1 + a1 * m0 + a2 * m1 + a3 * y2);
    }

} // namespace Voxium::Core
