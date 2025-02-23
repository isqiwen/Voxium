#include "Math/Size.h"

#include <cmath>
#include <format>

#include "Math/Hash.h"
#include "Math/MathUtility.h"
#include "Math/Vector.h"
#include "Math/VectorF.h"

namespace Voxium::Core
{
    const Size Size::Empty = Size();

    double SnapValue(double value, double scale) { return std::round(value / scale) * scale; }

    // ===================================================================
    // Public Member Function Definitions
    // ===================================================================

    bool Size::operator==(const Size& other) const { return IsEqual(other); }
    bool Size::operator!=(const Size& other) const { return !IsEqual(other); }

    bool Size::IsEqual(const Size& other) const
    {
        return IsNearlyEqual(Width, other.Width) && IsNearlyEqual(Height, other.Height);
    }

    Size Size::Ceiling() const { return Size(std::ceil(Width), std::ceil(Height)); }
    Size Size::Floor() const { return Size(std::floor(Width), std::floor(Height)); }
    Size Size::Round() const { return Size(std::round(Width), std::round(Height)); }
    Size Size::Scale(double scale) const { return Size(Width * scale, Height * scale); }
    Size Size::Snap(double scale) const { return Size(SnapValue(Width, scale), SnapValue(Height, scale)); }

    std::string Size::ToString(const std::string& format) const
    {
        std::string fmtStr = "(" + std::string("{0:") + format + "}" + "," + std::string("{1:") + format + "}" + ")";
        return std::vformat(fmtStr, std::make_format_args(Width, Height));
    }

    std::string Size::ToString() const { return std::format("({},{})", Width, Height); }

    Vector Size::ToVector() const { return Vector(Width, Height); }

    size_t Size::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, Width, Height);
        return hash;
    }

    Size operator*(const Size& size, const VectorF& v) { return Size(size.Width * v.X, size.Height * v.Y); }

    Size operator/(const Size& size, const VectorF& v) { return Size(size.Width / v.X, size.Height / v.Y); }

} // namespace Voxium::Core
