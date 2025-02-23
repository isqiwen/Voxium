#pragma once

#include <string>

#include "CoreMacros.h"

namespace Voxium::Core
{
    struct Vector3F;
    struct Vector4F;
    struct Matrix4;

#pragma pack(push, 1)
    struct CORE_API Matrix4F
    {
        float M11, M12, M13, M14;
        float M21, M22, M23, M24;
        float M31, M32, M33, M34;
        float M41, M42, M43, M44;

        static const Matrix4F Identity;

        Matrix4F();

        Matrix4F(float m11,
                 float m12,
                 float m13,
                 float m14,
                 float m21,
                 float m22,
                 float m23,
                 float m24,
                 float m31,
                 float m32,
                 float m33,
                 float m34,
                 float m41,
                 float m42,
                 float m43,
                 float m44);

        bool     operator==(const Matrix4F& other) const;
        bool     operator!=(const Matrix4F& other) const;
        Matrix4F operator*(const Matrix4F& b) const;

        bool        IsEqual(const Matrix4F& other) const;
        Matrix4F    Transpose() const;
        Matrix4F    Inverse() const;
        void        SetRow1(const Vector4F& v);
        void        SetRow2(const Vector4F& v);
        void        SetRow3(const Vector4F& v);
        void        SetRow4(const Vector4F& v);
        Vector4F    GetRow1() const;
        Vector4F    GetRow2() const;
        Vector4F    GetRow3() const;
        Vector4F    GetRow4() const;
        Matrix4     ToDouble() const;
        std::string ToString() const;

        static Matrix4F CreateOrthoProjection(float left, float top, float right, float bottom, float near, float far);
        static Matrix4F CreateLookAt(const Vector3F& eye, const Vector3F& target, const Vector3F& up);
        static Matrix4F CreatePerspectiveFieldOfView(float fovy, float aspect, float zNear, float zFar);
        static Matrix4F CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float zNear, float zFar);
        static Matrix4F CreateFPSView(Vector3F eye, float pitch, float yaw);
        static Matrix4F CreateRotationX(float rads);
        static Matrix4F CreateRotationY(float rads);
        static Matrix4F CreateRotationZ(float rads);
        static Matrix4F CreateRotationXY(float vX, float vY);
        static Matrix4F CreateRotationXZ(float vX, float vZ);
        static Matrix4F CreateRotationYZ(float vY, float vZ);
        static Matrix4F CreateTranslation(float x, float y, float z);
        static Matrix4F CreateTranslation(const Vector3F& vec);
        static Matrix4F CreateScale(float scaleX, float scaleY, float scaleZ);
        static Matrix4F CreateScale(float scaleFactor);
    };
#pragma pack(pop)

} // namespace Voxium::Core
