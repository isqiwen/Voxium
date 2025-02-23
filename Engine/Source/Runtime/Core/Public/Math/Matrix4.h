#pragma once

#include <string>

#include "CoreMacros.h"

namespace Voxium::Core
{
    struct Vector3;
    struct Vector4;
    struct Matrix4F;

#pragma pack(push, 1)
    struct CORE_API Matrix4
    {
        double M11, M12, M13, M14;
        double M21, M22, M23, M24;
        double M31, M32, M33, M34;
        double M41, M42, M43, M44;

        static const Matrix4 Identity;

        Matrix4();

        Matrix4(double m11,
                double m12,
                double m13,
                double m14,
                double m21,
                double m22,
                double m23,
                double m24,
                double m31,
                double m32,
                double m33,
                double m34,
                double m41,
                double m42,
                double m43,
                double m44);

        bool    operator==(const Matrix4& other) const;
        bool    operator!=(const Matrix4& other) const;
        Matrix4 operator*(const Matrix4& b) const;

        bool        IsEqual(const Matrix4& other) const;
        Matrix4     Transpose() const;
        Matrix4     Inverse() const;
        void        SetRow1(const Vector4& v);
        void        SetRow2(const Vector4& v);
        void        SetRow3(const Vector4& v);
        void        SetRow4(const Vector4& v);
        Vector4     GetRow1() const;
        Vector4     GetRow2() const;
        Vector4     GetRow3() const;
        Vector4     GetRow4() const;
        Matrix4F    ToFloat() const;
        std::string ToString() const;

        static Matrix4 CreateOrthoProjection(double left, double top, double right, double bottom, double near, double far);
        static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up);
        static Matrix4 CreatePerspectiveFieldOfView(double fovy, double aspect, double zNear, double zFar);
        static Matrix4 CreatePerspectiveOffCenter(double left, double right, double bottom, double top, double zNear, double zFar);
        static Matrix4 CreateFPSView(Vector3 eye, double pitch, double yaw);
        static Matrix4 CreateRotationX(double rads);
        static Matrix4 CreateRotationY(double rads);
        static Matrix4 CreateRotationZ(double rads);
        static Matrix4 CreateRotationXY(double vX, double vY);
        static Matrix4 CreateRotationXZ(double vX, double vZ);
        static Matrix4 CreateRotationYZ(double vY, double vZ);
        static Matrix4 CreateTranslation(double x, double y, double z);
        static Matrix4 CreateTranslation(const Vector3& vec);
        static Matrix4 CreateScale(double scaleX, double scaleY, double scaleZ);
        static Matrix4 CreateScale(double scaleFactor);
    };
#pragma pack(pop)

} // namespace Voxium::Core
