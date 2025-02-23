#include "Math/Int3.h"

#include <cmath>
#include <format>

#include "Math/MathUtility.h"
#include "Math/Hash.h"
#include "Math/Vector3.h"
#include "Math/Vector3F.h"

namespace Voxium::Core
{

    const int     Int3::INVALID_X   = -1;
    const Int3    Int3::Zero        = Int3(0, 0, 0);
    const Int3    Int3::Max         = Int3(VOXIUM_INT_MAX, VOXIUM_INT_MAX, VOXIUM_INT_MAX);
    const Int3    Int3::Invalid     = Int3(Int3::INVALID_X, 0, 0);
    const Int3    Int3::FullInvalid = Int3(Int3::INVALID_X, Int3::INVALID_X, Int3::INVALID_X);
    const Vector3 Int3::InvalidV    = Vector3(Int3::INVALID_X, 0, 0);

    Int3::Int3(const Vector3& v) : X(v.X), Y(v.Y), Z(v.Z) {}

    Int3::Int3(const Vector3F& v) : X(v.X), Y(v.Y), Z(v.Z) {}

    double Int3::Magnitude() const { return std::sqrt(static_cast<double>(X * X + Y * Y + Z * Z)); }

    double Int3::MagnitudeXZ() const { return std::sqrt(static_cast<double>(X * X + Z * Z)); }

    std::size_t Int3::GetHashCode() const
    {
        size_t hash = 0;
        HashCombine(hash, X, Y, Z);
        return hash;
    }

    Vector3 Int3::ToVector3() const { return Vector3(X, Y, Z); }

    Vector3F Int3::ToVector3F() const { return Vector3F(X, Y, Z); }

    std::string Int3::ToString() const { return std::format("({},{},{})", X, Y, Z); };

    bool Int3::Valid(const Vector3& v) { return v.X != Int3::INVALID_X; }

    Vector3 operator+(const Int3& v, const Vector3& i) { return Vector3(v.X + i.X, v.Y + i.Y, v.Z + i.Z); }

    Vector3 operator-(const Int3& v, const Vector3& i) { return Vector3(v.X - i.X, v.Y - i.Y, v.Z - i.Z); }

    Int3 operator*(const Int3& v, int i) { return Int3(v.X * i, v.Y * i, v.Z * i); }

    Int3 operator/(const Int3& v, int i) { return Int3(v.X / i, v.Y / i, v.Z / i); }

    bool operator>(const Vector3& v, const Int3& i) { return v.X > i.X && v.Y > i.Y && v.Z > i.Z; }

    bool operator<(const Vector3& v, const Int3& i) { return v.X < i.X && v.Y < i.Y && v.Z < i.Z; }

    bool operator>=(const Vector3& v, const Int3& i) { return v.X >= i.X && v.Y >= i.Y && v.Z >= i.Z; }

    bool operator<=(const Vector3& v, const Int3& i) { return v.X <= i.X && v.Y <= i.Y && v.Z <= i.Z; }

} // namespace Voxium::Core
