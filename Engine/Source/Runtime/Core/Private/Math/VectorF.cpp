#include "Math/VectorF.h"

#include <cmath>
#include <format>

#include "Math/Hash.h"
#include "Math/MathUtility.h"
#include "Math/Size.h"
#include "Math/Vector.h"

namespace Voxium::Core
{
    const VectorF VectorF::Zero = VectorF(0.0f, 0.0f);

    // ===================================================================
    // Public Member Function Definitions
    // ===================================================================

    bool VectorF::operator==(const VectorF& other) const { return IsEqual(other); }
    bool VectorF::operator!=(const VectorF& other) const { return !IsEqual(other); }

    bool VectorF::IsEqual(const VectorF& other) const { return IsNearlyEqual(X, other.X) && IsNearlyEqual(Y, other.Y); }

    float VectorF::Magnitude() const { return std::sqrt(X * X + Y * Y); }

    void VectorF::Normalize()
    {
        float mag = Magnitude();
        if (mag != 0)
        {
            X /= mag;
            Y /= mag;
        }
    }

    VectorF VectorF::Normalized() const
    {
        float mag = Magnitude();
        return (mag != 0) ? VectorF(X / mag, Y / mag) : *this;
    }

    std::size_t VectorF::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, X, Y);
        return hash;
    }

    Vector VectorF::ToDouble() const { return Vector(static_cast<double>(X), static_cast<double>(Y)); }

    Size VectorF::ToSize() const { return Size(static_cast<float>(X), static_cast<float>(Y)); }

    std::string VectorF::ToString() const { return std::format("[{:.2f},{:.2f}]", X, Y); }

    // ===================================================================
    // Static Member Function Definitions
    // ===================================================================

    float VectorF::Distance(const VectorF& a, const VectorF& b) { return (a - b).Magnitude(); }

    VectorF VectorF::Rotate(const VectorF& point, const VectorF& center, float angle)
    {
        VectorF delta      = point - center;
        float   startAngle = std::atan2(delta.Y, delta.X);
        float   newAngle   = startAngle + angle;
        float   mag        = delta.Magnitude();
        return VectorF(center.X + std::cos(newAngle) * mag, center.Y + std::sin(newAngle) * mag);
    }

} // namespace Voxium::Core
