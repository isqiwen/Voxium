#pragma once

#include <cmath>
#include <string>
#include <format>

namespace Voxium::Core {

    inline double SnapValue(double value, double scale) {
        return std::round(value / scale) * scale;
    }

    #pragma pack(push, 1)
    struct Size {
        double Width;
        double Height;

        constexpr Size(double width, double height)
            : Width(width), Height(height) {}

        constexpr Size(double size)
            : Width(size), Height(size) {}

        constexpr Size() : Width(0.0), Height(0.0) {}

        constexpr int GetHashCode() {}

        Size Ceiling() const {
            return Size(std::ceil(Width), std::ceil(Height));
        }

        Size Floor() const {
            return Size(std::floor(Width), std::floor(Height));
        }

        Size Round() const {
            return Size(std::round(Width), std::round(Height));
        }

        static const Size Empty;

        Size Scale(double scale) const {
            return Size(Width * scale, Height * scale);
        }

        std::string ToString(const std::string& format) const {
            return std::format("({:{fmt}},{:{fmt}})", Width, Height, fmt = format);
        }

        std::string ToString() const {
            return std::format("({},{})", Width, Height);
        }

        bool operator==(const Size& other) const {
            return Width == other.Width && Height == other.Height;
        }
        bool operator!=(const Size& other) const {
            return !(*this == other);
        }

        Size operator+(const Size& other) const {
            return Size(Width + other.Width, Height + other.Height);
        }

        Size operator-(const Size& other) const {
            return Size(Width - other.Width, Height - other.Height);
        }

        Size operator*(double scale) const {
            return Size(Width * scale, Height * scale);
        }

        Size operator/(double scale) const {
            return Size(Width / scale, Height / scale);
        }

        Size operator-() const {
            return Size(-Width, -Height);
        }

        Size Snap(double scale) const {
            return Size(SnapValue(Width, scale), SnapValue(Height, scale));
        }
    };
    #pragma pack(pop)

    inline const Size Size::Empty = Size();

} // namespace Voxium::Core
