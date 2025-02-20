#pragma once

#include <cmath>
#include <format>
#include <string>

#include "Core/Base/Hash.h"

namespace Voxium::Core
{

    struct EdgeInsets
    {
        double Left;
        double Right;
        double Top;
        double Bottom;

        constexpr EdgeInsets(double left, double top, double right, double bottom) :
            Left(left), Top(top), Right(right), Bottom(bottom)
        {}

        constexpr EdgeInsets(double all) : Left(all), Right(all), Top(all), Bottom(all) {}

        constexpr EdgeInsets() : Left(0), Right(0), Top(0), Bottom(0) {}

        constexpr size_t GetHashCode() const
        {
            size_t hash = 0;
            HashCombine(hash, Left, Right, Top, Bottom);
            return hash;
        }

        constexpr bool operator==(const EdgeInsets& other) const
        {
            constexpr double epsilon = std::numeric_limits<double>::epsilon();
            return std::abs(Left - other.Left) < epsilon && std::abs(Top - other.Top) < epsilon &&
                   std::abs(Right - other.Right) < epsilon && std::abs(Bottom - other.Bottom) < epsilon;
        }

        constexpr bool operator!=(const EdgeInsets& other) const { return !(*this == other); }

        constexpr EdgeInsets operator*(double scale) const
        {
            return EdgeInsets(Left * scale, Top * scale, Right * scale, Bottom * scale);
        }

        constexpr EdgeInsets operator+(const EdgeInsets& other) const
        {
            return EdgeInsets(Left + other.Left, Top + other.Top, Right + other.Right, Bottom + other.Bottom);
        }

        constexpr EdgeInsets operator-(const EdgeInsets& other) const
        {
            return EdgeInsets(Left - other.Left, Top - other.Top, Right - other.Right, Bottom - other.Bottom);
        }

        std::string ToString() const
        {
            return std::format("Top: {} Left: {} Bottom: {} Right: {}", Top, Left, Bottom, Right);
        }
    };

} // namespace Voxium::Core
