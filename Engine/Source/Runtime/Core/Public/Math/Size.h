#pragma once

#include <string>

#include "CoreMacros.h"

namespace Voxium::Core
{

    struct Vector;
    struct VectorF;

    double SnapValue(double value, double scale);

#pragma pack(push, 1)
    struct CORE_API Size
    {
        double            Width, Height;
        static const Size Empty;

        constexpr Size(double width, double height) : Width(width), Height(height) {}
        constexpr Size(double size) : Width(size), Height(size) {}
        constexpr Size() : Width(0.0), Height(0.0) {}

        // ===================================================================
        // Public Member Function
        // ===================================================================

        bool           operator==(const Size& other) const;
        bool           operator!=(const Size& other) const;
        constexpr Size operator+(const Size& other) const { return Size(Width + other.Width, Height + other.Height); }
        constexpr Size operator-(const Size& other) const { return Size(Width - other.Width, Height - other.Height); }
        constexpr Size operator-() const { return Size(-Width, -Height); }

        bool IsEqual(const Size& other) const;
        Size Ceiling() const;
        Size Floor() const;
        Size Round() const;
        Size Scale(double scale) const;
        Size Snap(double scale) const;

        size_t      GetHashCode() const;
        Vector      ToVector() const;
        std::string ToString() const;
        std::string ToString(const std::string& format) const;
    };
#pragma pack(pop)

    constexpr Size operator*(const Size& a, double scale) { return Size(a.Width * scale, a.Height * scale); }
    constexpr Size operator/(const Size& a, double scale) { return Size(a.Width / scale, a.Height / scale); }

    CORE_API Size operator*(const Size& size, const VectorF& v);
    CORE_API Size  operator/(const Size& size, const VectorF& v);

} // namespace Voxium::Core
