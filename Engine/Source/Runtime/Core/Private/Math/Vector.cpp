#include "Math/Vector.h"

#include <cmath>
#include <format>

#include "Math/Hash.h"
#include "Math/MathUtility.h"
#include "Math/Size.h"
#include "Math/VectorF.h"

namespace Voxium::Core
{

    const Vector Vector::Zero = Vector(0.0, 0.0);

    // ===================================================================
    // Public Member Function Definitions
    // ===================================================================

    bool Vector::operator==(const Vector& other) const { return IsEqual(other); }
    bool Vector::operator!=(const Vector& other) const { return !IsEqual(other); }

    bool Vector::IsEqual(const Vector& other) const { return IsNearlyEqual(X, other.X) && IsNearlyEqual(Y, other.Y); }

    double Vector::Magnitude() const { return std::sqrt(X * X + Y * Y); }

    void Vector::Normalize()
    {
        double mag = Magnitude();
        if (mag != 0)
        {
            X /= mag;
            Y /= mag;
        }
    }

    Vector Vector::Normalized() const
    {
        double mag = Magnitude();
        return (mag != 0) ? Vector(X / mag, Y / mag) : *this;
    }

    std::size_t Vector::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, X, Y);
        return hash;
    }

    VectorF Vector::ToFloat() const { return VectorF(static_cast<float>(X), static_cast<float>(Y)); }

    Size Vector::ToSize() const { return Size(X, Y); }

    std::string Vector::ToString() const { return std::format("[{:.2f},{:.2f}]", X, Y); }

    // ===================================================================
    // Static Member Function Definitions
    // ===================================================================

    double Vector::Distance(const Vector& a, const Vector& b) { return (a - b).Magnitude(); }

    Vector Vector::Rotate(const Vector& point, const Vector& center, double angle)
    {
        Vector delta      = point - center;
        double startAngle = std::atan2(delta.Y, delta.X);
        double newAngle   = startAngle + angle;
        double mag        = delta.Magnitude();
        return Vector(center.X + std::cos(newAngle) * mag, center.Y + std::sin(newAngle) * mag);
    }

} // namespace Voxium::Core
