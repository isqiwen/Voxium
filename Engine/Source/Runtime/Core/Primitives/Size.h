#pragma once

#include <cmath>
#include <format>
#include <limits>
#include <string>

#include "Core/Base/Hash.h"
#include "Vector.h"

namespace Voxium::Core
{

    inline double SnapValue(double value, double scale) { return std::round(value / scale) * scale; }

#pragma pack(push, 1)
    struct Size
    {
        double Width;
        double Height;

        static const Size Empty;

        constexpr Size(double width, double height) : Width(width), Height(height) {}
        constexpr Size(double size) : Width(size), Height(size) {}
        constexpr Size() : Width(0.0), Height(0.0) {}

        Size Ceiling() const { return Size(std::ceil(Width), std::ceil(Height)); }
        Size Floor() const { return Size(std::floor(Width), std::floor(Height)); }
        Size Round() const { return Size(std::round(Width), std::round(Height)); }
        Size Scale(double scale) const { return Size(Width * scale, Height * scale); }
        Size Snap(double scale) const { return Size(SnapValue(Width, scale), SnapValue(Height, scale)); }

        std::string ToString(const std::string& format) const
        {
            std::string fmtStr =
                "(" + std::string("{0:") + format + "}" + "," + std::string("{1:") + format + "}" + ")";
            return std::vformat(fmtStr, std::make_format_args(Width, Height));
        }

        std::string ToString() const { return std::format("({},{})", Width, Height); }

        constexpr Vector ToVector() const { return Vector(Width, Height); }

        constexpr size_t GetHashCode() const
        {
            size_t hash = 0;
            HashCombine(hash, Width, Height);
            return hash;
        }

        bool operator==(const Size& other) const
        {
            constexpr double epsilon = std::numeric_limits<double>::epsilon();
            return std::abs(Width - other.Width) < epsilon && std::abs(Height - other.Height) < epsilon;
        }

        bool operator!=(const Size& other) const { return !(*this == other); }
        Size operator+(const Size& other) const { return Size(Width + other.Width, Height + other.Height); }
        Size operator-(const Size& other) const { return Size(Width - other.Width, Height - other.Height); }
        Size operator*(double scale) const { return Size(Width * scale, Height * scale); }
        Size operator/(double scale) const { return Size(Width / scale, Height / scale); }
        Size operator-() const { return Size(-Width, -Height); }
    };
#pragma pack(pop)

    inline const Size Size::Empty = Size();

} // namespace Voxium::Core
