#pragma once

#include <string>

#include "CoreMacros.h"

#include "Vector3F.h"

namespace Voxium::Core
{
#pragma pack(push, 1)
    struct CORE_API Matrix3x4F
    {
        float M11, M12, M13, M14;
        float M21, M22, M23, M24;
        float M31, M32, M33, M34;

        static const Matrix3x4F Zero;

        constexpr Matrix3x4F(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34) :
            M11(m11), M12(m12), M13(m13), M14(m14), M21(m21), M22(m22), M23(m23), M24(m24), M31(m31), M32(m32), M33(m33), M34(m34)
        {}

        constexpr Matrix3x4F() : Matrix3x4F(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) {}
    };

    struct CORE_API Matrix3F
    {
        float M11, M12, M13;
        float M21, M22, M23;
        float M31, M32, M33;

        static const Matrix3F Zero;

        constexpr Matrix3F(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33) :
            M11(m11), M12(m12), M13(m13), M21(m21), M22(m22), M23(m23), M31(m31), M32(m32), M33(m33)
        {}

        constexpr Matrix3F() : Matrix3F(0, 0, 0, 0, 0, 0, 0, 0, 0) {}

        constexpr void SetRow1(const Vector3F& v)
        {
            M11 = v.X;
            M12 = v.Y;
            M13 = v.Z;
        }

        constexpr void SetRow2(const Vector3F& v)
        {
            M21 = v.X;
            M22 = v.Y;
            M23 = v.Z;
        }

        constexpr void SetRow3(const Vector3F& v)
        {
            M31 = v.X;
            M32 = v.Y;
            M33 = v.Z;
        }

        constexpr Vector3F GetRow1() const { return Vector3F(M11, M12, M13); }
        constexpr Vector3F GetRow2() const { return Vector3F(M21, M22, M23); }
        constexpr Vector3F GetRow3() const { return Vector3F(M31, M32, M33); }
    };
#pragma pack(pop)

} // namespace Voxium::Core
