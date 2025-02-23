#include "Math/Vector3F.h"

#include <cmath>
#include <format>
#include <limits>
#include <stdexcept>

#include "Math/Hash.h"
#include "Math/Int3.h"
#include "Math/MathUtility.h"
#include "Math/Matrix4.h"
#include "Math/Matrix4F.h"
#include "Math/Vector3.h"
#include "Math/Vector4F.h"

namespace Voxium::Core
{

    const Vector3F Vector3F::Zero = Vector3F(0, 0, 0);

    Vector3F::Vector3F(const Int3& v) : X(v.X), Y(v.Y), Z(v.Z) {}

    Vector3F::Vector3F(const Vector4F& v) : X(v.X), Y(v.Y), Z(v.Z) {}

    bool Vector3F::operator==(const Vector3F& other) { return IsEqual(other); }

    bool Vector3F::operator!=(const Vector3F& other) { return !IsEqual(other); }

    bool Vector3F::IsEqual(const Vector3F& other) const { return IsNearlyEqual(X, other.X) && IsNearlyEqual(Y, other.Y) && IsNearlyEqual(Z, other.Z); }

    Vector4F Vector3F::XYZW() const { return Vector4F(X, Y, Z, 1.0); }

    float Vector3F::Magnitude() const { return std::sqrt(X * X + Y * Y + Z * Z); }

    void Vector3F::Normalize()
    {
        float mag = Magnitude();
        if (mag == 0)
            throw std::runtime_error("Cannot normalize zero vector");
        float scale = 1.0 / mag;
        X *= scale;
        Y *= scale;
        Z *= scale;
    }

    Vector3F Vector3F::Normalized() const
    {
        float mag = Magnitude();
        if (mag == 0)
            return *this;
        float scale = 1.0 / mag;
        return Vector3F(X * scale, Y * scale, Z * scale);
    }

    std::size_t Vector3F::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, X, Y, Z);
        return hash;
    }

    Vector3 Vector3F::ToVector3() const { return Vector3(static_cast<double>(X), static_cast<double>(Y), static_cast<double>(Z)); }

    std::string Vector3F::ToString() const { return std::format("[{:.2f},{:.2f},{:.2f}]", X, Y, Z); }

    Vector3F Vector3F::FromPitchYaw(float pitch, float yaw)
    {
        float cosPitch = std::cos(pitch);
        return Vector3F(cosPitch * std::sin(yaw), std::sin(pitch), cosPitch * std::cos(yaw));
    }

    Vector3F operator*(const Vector3F& a, const Matrix4& b)
    {
        return Vector3F(
            a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + 1.0 * b.M41, a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + 1.0 * b.M42, a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + 1.0 * b.M43);
    }

    Vector3F operator*(const Vector3F& a, const Matrix4F& b)
    {
        return Vector3F(
            a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + 1.0f * b.M41, a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + 1.0f * b.M42, a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + 1.0f * b.M43);
    }

} // namespace Voxium::Core
