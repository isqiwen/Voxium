#pragma once

#include <string>

#include "CoreMacros.h"


namespace Voxium::Core
{
    struct VectorF;
    struct Vector3F;

#pragma pack(push, 1)
    struct CORE_API Vector4F
    {
        float                 X, Y, Z, W;
        static const Vector4F Zero;

        Vector4F(float x, float y, float z, float w);
        Vector4F(const Vector3F& v, float w = 1.0);

        bool operator==(const Vector4F& other) const;
        bool operator!=(const Vector4F& other) const;

        bool        IsEqual(const Vector4F& other) const;
        VectorF     XY() const;
        Vector3F    XYZ() const;
        std::size_t GetHashCode() const;
        std::string ToString() const;
    };
#pragma pack(pop)

    struct Matrix4;
    struct Matrix4F;

    CORE_API Vector4F operator*(const Vector4F& a, const Matrix4& b);
    CORE_API Vector4F operator*(const Vector4F& a, const Matrix4F& b);

} // namespace Voxium::Core
