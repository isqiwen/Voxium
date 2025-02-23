#include "Math/Matrix4F.h"

#include <cmath>
#include <format>
#include <stdexcept>

#include "Math/Hash.h"
#include "Math/MathUtility.h"
#include "Math/Matrix4.h"
#include "Math/Vector3F.h"
#include "Math/Vector4F.h"

namespace Voxium::Core
{
    const Matrix4F Matrix4F::Identity = Matrix4F(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    Matrix4F::Matrix4F(float m11,
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
                       float m44) :
        M11(m11), M12(m12), M13(m13), M14(m14), M21(m21), M22(m22), M23(m23), M24(m24), M31(m31), M32(m32), M33(m33), M34(m34), M41(m41), M42(m42), M43(m43),
        M44(m44)
    {}

    Matrix4F::Matrix4F() : M11(0), M12(0), M13(0), M14(0), M21(0), M22(0), M23(0), M24(0), M31(0), M32(0), M33(0), M34(0), M41(0), M42(0), M43(0), M44(0) {}

    bool Matrix4F::operator==(const Matrix4F& other) const { return IsEqual(other); }

    bool Matrix4F::operator!=(const Matrix4F& other) const { return !IsEqual(other); }

    Matrix4F Matrix4F::operator*(const Matrix4F& b) const
    {
        return Matrix4F(M11 * b.M11 + M12 * b.M21 + M13 * b.M31 + M14 * b.M41,
                        M11 * b.M12 + M12 * b.M22 + M13 * b.M32 + M14 * b.M42,
                        M11 * b.M13 + M12 * b.M23 + M13 * b.M33 + M14 * b.M43,
                        M11 * b.M14 + M12 * b.M24 + M13 * b.M34 + M14 * b.M44,

                        M21 * b.M11 + M22 * b.M21 + M23 * b.M31 + M24 * b.M41,
                        M21 * b.M12 + M22 * b.M22 + M23 * b.M32 + M24 * b.M42,
                        M21 * b.M13 + M22 * b.M23 + M23 * b.M33 + M24 * b.M43,
                        M21 * b.M14 + M22 * b.M24 + M23 * b.M34 + M24 * b.M44,

                        M31 * b.M11 + M32 * b.M21 + M33 * b.M31 + M34 * b.M41,
                        M31 * b.M12 + M32 * b.M22 + M33 * b.M32 + M34 * b.M42,
                        M31 * b.M13 + M32 * b.M23 + M33 * b.M33 + M34 * b.M43,
                        M31 * b.M14 + M32 * b.M24 + M33 * b.M34 + M34 * b.M44,

                        M41 * b.M11 + M42 * b.M21 + M43 * b.M31 + M44 * b.M41,
                        M41 * b.M12 + M42 * b.M22 + M43 * b.M32 + M44 * b.M42,
                        M41 * b.M13 + M42 * b.M23 + M43 * b.M33 + M44 * b.M43,
                        M41 * b.M14 + M42 * b.M24 + M43 * b.M34 + M44 * b.M44);
    }

    bool Matrix4F::IsEqual(const Matrix4F& other) const
    {
        return IsNearlyEqual(M11, other.M11) && IsNearlyEqual(M12, other.M12) && IsNearlyEqual(M13, other.M13) && IsNearlyEqual(M14, other.M14) &&
               IsNearlyEqual(M21, other.M21) && IsNearlyEqual(M22, other.M22) && IsNearlyEqual(M23, other.M23) && IsNearlyEqual(M24, other.M24) &&
               IsNearlyEqual(M31, other.M31) && IsNearlyEqual(M32, other.M32) && IsNearlyEqual(M33, other.M33) && IsNearlyEqual(M34, other.M34) &&
               IsNearlyEqual(M41, other.M41) && IsNearlyEqual(M42, other.M42) && IsNearlyEqual(M43, other.M43) && IsNearlyEqual(M44, other.M44);
    }

    Matrix4F Matrix4F::Transpose() const { return Matrix4F(M11, M21, M31, M41, M12, M22, M32, M42, M13, M23, M33, M43, M14, M24, M34, M44); }

    Matrix4F Matrix4F::Inverse() const
    {
        Matrix4F inv(M22 * M33 * M44 - M22 * M34 * M43 - M32 * M23 * M44 + M32 * M24 * M43 + M42 * M23 * M34 - M42 * M24 * M33,

                     -M12 * M33 * M44 + M12 * M34 * M43 + M32 * M13 * M44 - M32 * M14 * M43 - M42 * M13 * M34 + M42 * M14 * M33,

                     M12 * M23 * M44 - M12 * M24 * M43 - M22 * M13 * M44 + M22 * M14 * M43 + M42 * M13 * M24 - M42 * M14 * M23,

                     -M12 * M23 * M34 + M12 * M24 * M33 + M22 * M13 * M34 - M22 * M14 * M33 - M32 * M13 * M24 + M32 * M14 * M23,

                     -M21 * M33 * M44 + M21 * M34 * M43 + M31 * M23 * M44 - M31 * M24 * M43 - M41 * M23 * M34 + M41 * M24 * M33,

                     M11 * M33 * M44 - M11 * M34 * M43 - M31 * M13 * M44 + M31 * M14 * M43 + M41 * M13 * M34 - M41 * M14 * M33,

                     -M11 * M23 * M44 + M11 * M24 * M43 + M21 * M13 * M44 - M21 * M14 * M43 - M41 * M13 * M24 + M41 * M14 * M23,

                     M11 * M23 * M34 - M11 * M24 * M33 - M21 * M13 * M34 + M21 * M14 * M33 + M31 * M13 * M24 - M31 * M14 * M23,

                     M21 * M32 * M44 - M21 * M34 * M42 - M31 * M22 * M44 + M31 * M24 * M42 + M41 * M22 * M34 - M41 * M24 * M32,

                     -M11 * M32 * M44 + M11 * M34 * M42 + M31 * M12 * M44 - M31 * M14 * M42 - M41 * M12 * M34 + M41 * M14 * M32,

                     M11 * M22 * M44 - M11 * M24 * M42 - M21 * M12 * M44 + M21 * M14 * M42 + M41 * M12 * M24 - M41 * M14 * M22,

                     -M11 * M22 * M34 + M11 * M24 * M32 + M21 * M12 * M34 - M21 * M14 * M32 - M31 * M12 * M24 + M31 * M14 * M22,

                     -M21 * M32 * M43 + M21 * M33 * M42 + M31 * M22 * M43 - M31 * M23 * M42 - M41 * M22 * M33 + M41 * M23 * M32,

                     M11 * M32 * M43 - M11 * M33 * M42 - M31 * M12 * M43 + M31 * M13 * M42 + M41 * M12 * M33 - M41 * M13 * M32,

                     -M11 * M22 * M43 + M11 * M23 * M42 + M21 * M12 * M43 - M21 * M13 * M42 - M41 * M12 * M23 + M41 * M13 * M22,

                     M11 * M22 * M33 - M11 * M23 * M32 - M21 * M12 * M33 + M21 * M13 * M32 + M31 * M12 * M23 - M31 * M13 * M22);
        float    det = M11 * inv.M11 + M12 * inv.M21 + M13 * inv.M31 + M14 * inv.M41;

        if (IsNearlyEqual(det, 0))
            throw std::runtime_error("No inverse matrix");

        det = 1.0 / det;
        inv.M11 *= det;
        inv.M12 *= det;
        inv.M13 *= det;
        inv.M14 *= det;
        inv.M21 *= det;
        inv.M22 *= det;
        inv.M23 *= det;
        inv.M24 *= det;
        inv.M31 *= det;
        inv.M32 *= det;
        inv.M33 *= det;
        inv.M34 *= det;
        inv.M41 *= det;
        inv.M42 *= det;
        inv.M43 *= det;
        inv.M44 *= det;
        return inv;
    }

    void Matrix4F::SetRow1(const Vector4F& v)
    {
        M11 = v.X;
        M12 = v.Y;
        M13 = v.Z;
        M14 = v.W;
    }

    void Matrix4F::SetRow2(const Vector4F& v)
    {
        M21 = v.X;
        M22 = v.Y;
        M23 = v.Z;
        M24 = v.W;
    }

    void Matrix4F::SetRow3(const Vector4F& v)
    {
        M31 = v.X;
        M32 = v.Y;
        M33 = v.Z;
        M34 = v.W;
    }

    void Matrix4F::SetRow4(const Vector4F& v)
    {
        M41 = v.X;
        M42 = v.Y;
        M43 = v.Z;
        M44 = v.W;
    }

    Vector4F Matrix4F::GetRow1() const { return Vector4F(M11, M12, M13, M14); }
    Vector4F Matrix4F::GetRow2() const { return Vector4F(M21, M22, M23, M24); }
    Vector4F Matrix4F::GetRow3() const { return Vector4F(M31, M32, M33, M34); }
    Vector4F Matrix4F::GetRow4() const { return Vector4F(M41, M42, M43, M44); }

    Matrix4 Matrix4F::ToDouble() const
    {
        Matrix4 f;
        f.M11 = static_cast<double>(M11);
        f.M12 = static_cast<double>(M12);
        f.M13 = static_cast<double>(M13);
        f.M14 = static_cast<double>(M14);
        f.M21 = static_cast<double>(M21);
        f.M22 = static_cast<double>(M22);
        f.M23 = static_cast<double>(M23);
        f.M24 = static_cast<double>(M24);
        f.M31 = static_cast<double>(M31);
        f.M32 = static_cast<double>(M32);
        f.M33 = static_cast<double>(M33);
        f.M34 = static_cast<double>(M34);
        f.M41 = static_cast<double>(M41);
        f.M42 = static_cast<double>(M42);
        f.M43 = static_cast<double>(M43);
        f.M44 = static_cast<double>(M44);
        return f;
    }

    std::string Matrix4F::ToString() const
    {
        return std::format("[[{:.2f}, {:.2f}, {:.2f}, {:.2f}],\n"
                           " [{:.2f}, {:.2f}, {:.2f}, {:.2f}],\n"
                           " [{:.2f}, {:.2f}, {:.2f}, {:.2f}],\n"
                           " [{:.2f}, {:.2f}, {:.2f}, {:.2f}]]",
                           M11,
                           M12,
                           M13,
                           M14,
                           M21,
                           M22,
                           M23,
                           M24,
                           M31,
                           M32,
                           M33,
                           M34,
                           M41,
                           M42,
                           M43,
                           M44);
    }

    Matrix4F Matrix4F::CreateOrthoProjection(float left, float top, float right, float bottom, float near, float far)
    {
        Matrix4F projection;
        projection.M11 = 2.0 / (right - left);
        projection.M12 = 0.0;
        projection.M13 = 0.0;
        projection.M14 = 0.0;

        projection.M21 = 0.0;
        projection.M22 = 2.0 / (top - bottom);
        projection.M23 = 0.0;
        projection.M24 = 0.0;

        projection.M31 = 0.0;
        projection.M32 = 0.0;
        projection.M33 = -2.0 / (far - near);
        projection.M34 = 0.0;

        projection.M41 = -(right + left) / (right - left);
        projection.M42 = -(top + bottom) / (top - bottom);
        projection.M43 = -(far + near) / (far - near);
        projection.M44 = 1.0;
        return projection;
    }

    Matrix4F Matrix4F::CreateLookAt(const Vector3F& eye, const Vector3F& target, const Vector3F& up)
    {
        Vector3F z = (eye - target).Normalized();
        Vector3F x = Vector3F::CrossProduct(up, z).Normalized();
        Vector3F y = Vector3F::CrossProduct(z, x).Normalized();

        Matrix4F rot(x.X, y.X, z.X, 0.0, x.Y, y.Y, z.Y, 0.0, x.Z, y.Z, z.Z, 0.0, 0, 0, 0, 1.0);
        Matrix4F trans = CreateTranslation(Vector3F(-eye.X, -eye.Y, -eye.Z));
        return trans * rot;
    }

    Matrix4F Matrix4F::CreatePerspectiveFieldOfView(float fovy, float aspect, float zNear, float zFar)
    {
        if (IsLessThanOrEqual(fovy, 0) || IsGreaterThan(fovy, VOXIUM_FLOAT_PI))
            throw std::out_of_range("fovy");
        if (IsLessThanOrEqual(aspect, 0))
            throw std::out_of_range("aspect");
        if (IsLessThanOrEqual(zNear, 0))
            throw std::out_of_range("zNear");
        if (IsLessThanOrEqual(zFar, 0))
            throw std::out_of_range("zFar");
        if (IsGreaterThanOrEqual(zNear, zFar))
            throw std::out_of_range("zNear");

        float yMax = zNear * std::tan(0.5 * fovy);
        float yMin = -yMax;
        float xMin = yMin * aspect;
        float xMax = yMax * aspect;
        return CreatePerspectiveOffCenter(xMin, xMax, yMin, yMax, zNear, zFar);
    }

    Matrix4F Matrix4F::CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        if (IsLessThanOrEqual(zNear, 0))
            throw std::out_of_range("zNear");
        if (IsLessThanOrEqual(zFar, 0))
            throw std::out_of_range("zFar");
        if (IsGreaterThanOrEqual(zNear, zFar))
            throw std::out_of_range("zNear");

        float x = (2.0 * zNear) / (right - left);
        float y = (2.0 * zNear) / (top - bottom);
        float a = (right + left) / (right - left);
        float b = (top + bottom) / (top - bottom);
        float c = -(zFar + zNear) / (zFar - zNear);
        float d = -(2.0 * zFar * zNear) / (zFar - zNear);
        return Matrix4F(x, 0, 0, 0, 0, y, 0, 0, a, b, c, -1, 0, 0, d, 0);
    }

    Matrix4F Matrix4F::CreateFPSView(Vector3F eye, float pitch, float yaw)
    {
        eye.X = -eye.X;
        eye.Y = -eye.Y;

        float cosPitch = std::cos(pitch);
        float sinPitch = std::sin(pitch);
        float cosYaw   = std::cos(-yaw);
        float sinYaw   = std::sin(-yaw);

        Vector3F xaxis {cosYaw, 0, -sinYaw};
        Vector3F yaxis {sinYaw * sinPitch, cosPitch, cosYaw * sinPitch};
        Vector3F zaxis {sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw};

        Matrix4F rot(xaxis.X, yaxis.X, zaxis.X, 0.0, xaxis.Y, yaxis.Y, zaxis.Y, 0.0, xaxis.Z, yaxis.Z, zaxis.Z, 0.0, 0, 0, 0, 1.0);
        Matrix4F trans = CreateTranslation(Vector3F(-eye.X, -eye.Y, -eye.Z));
        return trans * rot;
    }

    Matrix4F Matrix4F::CreateRotationX(float rads)
    {
        float cosVal = std::cos(rads);
        float sinVal = std::sin(rads);
        return Matrix4F(1, 0, 0, 0, 0, cosVal, sinVal, 0, 0, -sinVal, cosVal, 0, 0, 0, 0, 1);
    }

    Matrix4F Matrix4F::CreateRotationY(float rads)
    {
        float cosVal = std::cos(rads);
        float sinVal = std::sin(rads);
        return Matrix4F(cosVal, 0, -sinVal, 0, 0, 1, 0, 0, sinVal, 0, cosVal, 0, 0, 0, 0, 1);
    }

    Matrix4F Matrix4F::CreateRotationZ(float rads)
    {
        float cosVal = std::cos(rads);
        float sinVal = std::sin(rads);
        return Matrix4F(cosVal, sinVal, 0, 0, -sinVal, cosVal, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    }

    Matrix4F Matrix4F::CreateRotationXY(float vX, float vY)
    {
        Matrix4F m  = Identity;
        float    sX = std::sin(vX);
        float    cX = std::cos(vX);
        float    sY = std::sin(vY);
        float    cY = std::cos(vY);
        m.M11       = cY;
        m.M12       = 0;
        m.M13       = -sY;

        m.M21 = sX * sY;
        m.M22 = cX;
        m.M23 = sX * cY;

        m.M31 = sY * cX;
        m.M32 = -sX;
        m.M33 = cX * cY;
        return m;
    }

    Matrix4F Matrix4F::CreateRotationXZ(float vX, float vZ)
    {
        Matrix4F m  = Identity;
        float    sX = std::sin(vX);
        float    cX = std::cos(vX);
        float    sZ = std::sin(vZ);
        float    cZ = std::cos(vZ);
        m.M11       = cZ;
        m.M12       = sZ;

        m.M21 = -cX * sZ;
        m.M22 = cX * cZ;
        m.M23 = sX;

        m.M31 = sX * sZ;
        m.M32 = -sX * cZ;
        m.M33 = cX;
        return m;
    }

    Matrix4F Matrix4F::CreateRotationYZ(float vY, float vZ)
    {
        Matrix4F m  = Identity;
        float    sY = std::sin(vY);
        float    cY = std::cos(vY);
        float    sZ = std::sin(vZ);
        float    cZ = std::cos(vZ);
        m.M11       = cY * cZ;
        m.M12       = cY * sZ;
        m.M13       = -sY;

        m.M21 = -sZ;
        m.M22 = cZ;
        m.M23 = 0;

        m.M31 = sY * cZ;
        m.M32 = sY * sZ;
        m.M33 = cY;
        return m;
    }

    Matrix4F Matrix4F::CreateTranslation(float x, float y, float z) { return Matrix4F(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1); }

    Matrix4F Matrix4F::CreateTranslation(const Vector3F& vec) { return CreateTranslation(vec.X, vec.Y, vec.Z); }

    Matrix4F Matrix4F::CreateScale(float scaleX, float scaleY, float scaleZ) { return Matrix4F(scaleX, 0, 0, 0, 0, scaleY, 0, 0, 0, 0, scaleZ, 0, 0, 0, 0, 1); }

    Matrix4F Matrix4F::CreateScale(float scaleFactor) { return CreateScale(scaleFactor, scaleFactor, scaleFactor); }

} // namespace Voxium::Core
