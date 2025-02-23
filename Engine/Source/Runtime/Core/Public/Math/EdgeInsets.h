#pragma once

#include <string>

#include "CoreMacros.h"

namespace Voxium::Core
{

    struct CORE_API EdgeInsets
    {
        double Left, Right, Top, Bottom;

        constexpr EdgeInsets(double left, double top, double right, double bottom) : Left(left), Top(top), Right(right), Bottom(bottom) {}

        constexpr EdgeInsets(double all) : Left(all), Right(all), Top(all), Bottom(all) {}

        constexpr EdgeInsets() : Left(0), Right(0), Top(0), Bottom(0) {}

        bool operator==(const EdgeInsets& other) const;
        bool operator!=(const EdgeInsets& other) const;

        constexpr EdgeInsets operator*(double scale) const { return EdgeInsets(Left * scale, Top * scale, Right * scale, Bottom * scale); }

        constexpr EdgeInsets operator+(const EdgeInsets& other) const
        {
            return EdgeInsets(Left + other.Left, Top + other.Top, Right + other.Right, Bottom + other.Bottom);
        }

        constexpr EdgeInsets operator-(const EdgeInsets& other) const
        {
            return EdgeInsets(Left - other.Left, Top - other.Top, Right - other.Right, Bottom - other.Bottom);
        }

        bool        IsEqual(const EdgeInsets& other) const;
        size_t      GetHashCode() const;
        std::string ToString() const;
    };

} // namespace Voxium::Core
