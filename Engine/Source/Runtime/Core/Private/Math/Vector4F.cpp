#include "Math/Vector4F.h"

#include <cmath>
#include <format>
#include <limits>
#include <stdexcept>

#include "Math/Hash.h"
#include "Math/MathUtility.h"
#include "Math/Matrix4.h"
#include "Math/Matrix4F.h"
#include "Math/Vector3F.h"
#include "Math/VectorF.h"

namespace Voxium::Core
{

    const Vector4F Vector4F::Zero = Vector4F(0, 0, 0, 0);

    Vector4F::Vector4F(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) {}

    Vector4F::Vector4F(const Vector3F& v, float w) : X(v.X), Y(v.Y), Z(v.Z), W(w) {}

    bool Vector4F::operator==(const Vector4F& other) const { return IsEqual(other); }
    bool Vector4F::operator!=(const Vector4F& other) const { return !IsEqual(other); }

    VectorF  Vector4F::XY() const { return VectorF(X, Y); }
    Vector3F Vector4F::XYZ() const { return Vector3F(X, Y, Z); }

    bool Vector4F::IsEqual(const Vector4F& other) const
    {
        return IsNearlyEqual(X, other.X) && IsNearlyEqual(Y, other.Y) && IsNearlyEqual(Z, other.Z) && IsNearlyEqual(W, other.W);
    }

    std::size_t Vector4F::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, X, Y, Z, W);
        return hash;
    }

    std::string Vector4F::ToString() const { return std::format("[{:.2f},{:.2f},{:.2f},{:.2f}]", X, Y, Z, W); }

    Vector4F operator*(const Vector4F& a, const Matrix4& b)
    {
        return Vector4F(a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + a.W * b.M41,
                        a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + a.W * b.M42,
                        a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + a.W * b.M43,
                        a.X * b.M14 + a.Y * b.M24 + a.Z * b.M34 + a.W * b.M44);
    }

    Vector4F operator*(const Vector4F& a, const Matrix4F& b)
    {
        return Vector4F(a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + a.W * b.M41,
                        a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + a.W * b.M42,
                        a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + a.W * b.M43,
                        a.X * b.M14 + a.Y * b.M24 + a.Z * b.M34 + a.W * b.M44);
    }

} // namespace Voxium::Core
