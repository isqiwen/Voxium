#pragma once

#include <string>

#include "CoreMacros.h"

namespace Voxium::Core
{

    struct Vector;
    struct Vector3;

#pragma pack(push, 1)
    struct CORE_API Vector4
    {
        double               X, Y, Z, W;
        static const Vector4 Zero;

        Vector4(double x, double y, double z, double w);
        Vector4(const Vector3& v, double w = 1.0);

        bool operator==(const Vector4& other) const;
        bool operator!=(const Vector4& other) const;

        bool        IsEqual(const Vector4& other) const;
        Vector      XY() const;
        Vector3     XYZ() const;
        std::size_t GetHashCode() const;
        std::string ToString() const;
    };
#pragma pack(pop)

    struct Matrix4;
    struct Matrix4F;

    CORE_API Vector4 operator*(const Vector4& a, const Matrix4& b);
    CORE_API Vector4 operator*(const Vector4& a, const Matrix4F& b);

} // namespace Voxium::Core
