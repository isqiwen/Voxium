#include "Math/Vector3.h"

#include <cmath>
#include <format>
#include <limits>
#include <stdexcept>

#include "Math/Hash.h"
#include "Math/Int3.h"
#include "Math/MathUtility.h"
#include "Math/Matrix4.h"
#include "Math/Matrix4F.h"
#include "Math/Vector3F.h"
#include "Math/Vector4.h"

namespace Voxium::Core
{

    const Vector3 Vector3::Zero = Vector3(0, 0, 0);

    Vector3::Vector3(const Int3& v) : X(v.X), Y(v.Y), Z(v.Z) {}

    Vector3::Vector3(const Vector4& v) : X(v.X), Y(v.Y), Z(v.Z) {}

    bool Vector3::operator==(const Vector3& other) { return IsEqual(other); }

    bool Vector3::operator!=(const Vector3& other) { return !IsEqual(other); }

    bool Vector3::IsEqual(const Vector3& other) const { return IsNearlyEqual(X, other.X) && IsNearlyEqual(Y, other.Y) && IsNearlyEqual(Z, other.Z); }

    Vector4 Vector3::XYZW() const { return Vector4(X, Y, Z, 1.0); }

    double Vector3::Magnitude() const { return std::sqrt(X * X + Y * Y + Z * Z); }

    void Vector3::Normalize()
    {
        double mag = Magnitude();
        if (mag == 0)
            throw std::runtime_error("Cannot normalize zero vector");
        double scale = 1.0 / mag;
        X *= scale;
        Y *= scale;
        Z *= scale;
    }

    Vector3 Vector3::Normalized() const
    {
        double mag = Magnitude();
        if (mag == 0)
            return *this;
        double scale = 1.0 / mag;
        return Vector3(X * scale, Y * scale, Z * scale);
    }

    std::size_t Vector3::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, X, Y, Z);
        return hash;
    }

    Vector3F Vector3::ToVector3F() const { return Vector3F(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Z)); }

    std::string Vector3::ToString() const { return std::format("[{:.2f},{:.2f},{:.2f}]", X, Y, Z); }

    Vector3 Vector3::FromPitchYaw(double pitch, double yaw)
    {
        double cosPitch = std::cos(pitch);
        return Vector3(cosPitch * std::sin(yaw), std::sin(pitch), cosPitch * std::cos(yaw));
    }

    Vector3 operator*(const Vector3& a, const Matrix4& b)
    {
        return Vector3(
            a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + 1.0 * b.M41, a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + 1.0 * b.M42, a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + 1.0 * b.M43);
    }

    Vector3 operator*(const Vector3& a, const Matrix4F& b)
    {
        return Vector3(
            a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + 1.0f * b.M41, a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + 1.0f * b.M42, a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + 1.0f * b.M43);
    }

} // namespace Voxium::Core
