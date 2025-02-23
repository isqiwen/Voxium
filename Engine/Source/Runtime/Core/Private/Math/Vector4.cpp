#include "Math/Vector4.h"

#include <cmath>
#include <format>
#include <limits>
#include <stdexcept>

#include "Math/Hash.h"
#include "Math/MathUtility.h"
#include "Math/Matrix4.h"
#include "Math/Matrix4F.h"
#include "Math/Vector.h"
#include "Math/Vector3.h"

namespace Voxium::Core
{

    const Vector4 Vector4::Zero = Vector4(0, 0, 0, 0);

    Vector4::Vector4(double x, double y, double z, double w) : X(x), Y(y), Z(z), W(w) {}

    Vector4::Vector4(const Vector3& v, double w) : X(v.X), Y(v.Y), Z(v.Z), W(w) {}

    bool Vector4::operator==(const Vector4& other) const { return IsEqual(other); }
    bool Vector4::operator!=(const Vector4& other) const { return !IsEqual(other); }

    bool Vector4::IsEqual(const Vector4& other) const
    {
        return IsNearlyEqual(X, other.X) && IsNearlyEqual(Y, other.Y) && IsNearlyEqual(Z, other.Z) && IsNearlyEqual(W, other.W);
    }

    Vector Vector4::XY() const { return Vector(X, Y); }

    Vector3 Vector4::XYZ() const { return Vector3(X, Y, Z); }

    std::size_t Vector4::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, X, Y, Z, W);
        return hash;
    }

    std::string Vector4::ToString() const { return std::format("[{:.2f},{:.2f},{:.2f},{:.2f}]", X, Y, Z, W); }

    Vector4 operator*(const Vector4& a, const Matrix4& b)
    {
        return Vector4(a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + a.W * b.M41,
                       a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + a.W * b.M42,
                       a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + a.W * b.M43,
                       a.X * b.M14 + a.Y * b.M24 + a.Z * b.M34 + a.W * b.M44);
    }

    Vector4 operator*(const Vector4& a, const Matrix4F& b)
    {
        return Vector4(a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + a.W * b.M41,
                       a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + a.W * b.M42,
                       a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + a.W * b.M43,
                       a.X * b.M14 + a.Y * b.M24 + a.Z * b.M34 + a.W * b.M44);
    }

} // namespace Voxium::Core
