#include "Math/Matrix4.h"

#include <cmath>
#include <format>
#include <stdexcept>

#include "Math/Hash.h"
#include "Math/MathUtility.h"
#include "Math/Matrix4F.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

namespace Voxium::Core
{
    const Matrix4 Matrix4::Identity = Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

    Matrix4::Matrix4(double m11,
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
                     double m44) :
        M11(m11), M12(m12), M13(m13), M14(m14), M21(m21), M22(m22), M23(m23), M24(m24), M31(m31), M32(m32), M33(m33), M34(m34), M41(m41), M42(m42), M43(m43),
        M44(m44)
    {}

    Matrix4::Matrix4() : M11(0), M12(0), M13(0), M14(0), M21(0), M22(0), M23(0), M24(0), M31(0), M32(0), M33(0), M34(0), M41(0), M42(0), M43(0), M44(0) {}

    bool Matrix4::operator==(const Matrix4& other) const { return IsEqual(other); }

    bool Matrix4::operator!=(const Matrix4& other) const { return !IsEqual(other); }

    Matrix4 Matrix4::operator*(const Matrix4& b) const
    {
        return Matrix4(M11 * b.M11 + M12 * b.M21 + M13 * b.M31 + M14 * b.M41,
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

    bool Matrix4::IsEqual(const Matrix4& other) const
    {
        return IsNearlyEqual(M11, other.M11) && IsNearlyEqual(M12, other.M12) && IsNearlyEqual(M13, other.M13) && IsNearlyEqual(M14, other.M14) &&
               IsNearlyEqual(M21, other.M21) && IsNearlyEqual(M22, other.M22) && IsNearlyEqual(M23, other.M23) && IsNearlyEqual(M24, other.M24) &&
               IsNearlyEqual(M31, other.M31) && IsNearlyEqual(M32, other.M32) && IsNearlyEqual(M33, other.M33) && IsNearlyEqual(M34, other.M34) &&
               IsNearlyEqual(M41, other.M41) && IsNearlyEqual(M42, other.M42) && IsNearlyEqual(M43, other.M43) && IsNearlyEqual(M44, other.M44);
    }

    Matrix4 Matrix4::Transpose() const { return Matrix4(M11, M21, M31, M41, M12, M22, M32, M42, M13, M23, M33, M43, M14, M24, M34, M44); }

    Matrix4 Matrix4::Inverse() const
    {
        Matrix4 inv(M22 * M33 * M44 - M22 * M34 * M43 - M32 * M23 * M44 + M32 * M24 * M43 + M42 * M23 * M34 - M42 * M24 * M33,

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
        double  det = M11 * inv.M11 + M12 * inv.M21 + M13 * inv.M31 + M14 * inv.M41;

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

    void Matrix4::SetRow1(const Vector4& v)
    {
        M11 = v.X;
        M12 = v.Y;
        M13 = v.Z;
        M14 = v.W;
    }

    void Matrix4::SetRow2(const Vector4& v)
    {
        M21 = v.X;
        M22 = v.Y;
        M23 = v.Z;
        M24 = v.W;
    }

    void Matrix4::SetRow3(const Vector4& v)
    {
        M31 = v.X;
        M32 = v.Y;
        M33 = v.Z;
        M34 = v.W;
    }

    void Matrix4::SetRow4(const Vector4& v)
    {
        M41 = v.X;
        M42 = v.Y;
        M43 = v.Z;
        M44 = v.W;
    }

    Vector4 Matrix4::GetRow1() const { return Vector4(M11, M12, M13, M14); }
    Vector4 Matrix4::GetRow2() const { return Vector4(M21, M22, M23, M24); }
    Vector4 Matrix4::GetRow3() const { return Vector4(M31, M32, M33, M34); }
    Vector4 Matrix4::GetRow4() const { return Vector4(M41, M42, M43, M44); }

    Matrix4F Matrix4::ToFloat() const
    {
        Matrix4F f;
        f.M11 = static_cast<float>(M11);
        f.M12 = static_cast<float>(M12);
        f.M13 = static_cast<float>(M13);
        f.M14 = static_cast<float>(M14);
        f.M21 = static_cast<float>(M21);
        f.M22 = static_cast<float>(M22);
        f.M23 = static_cast<float>(M23);
        f.M24 = static_cast<float>(M24);
        f.M31 = static_cast<float>(M31);
        f.M32 = static_cast<float>(M32);
        f.M33 = static_cast<float>(M33);
        f.M34 = static_cast<float>(M34);
        f.M41 = static_cast<float>(M41);
        f.M42 = static_cast<float>(M42);
        f.M43 = static_cast<float>(M43);
        f.M44 = static_cast<float>(M44);
        return f;
    }

    std::string Matrix4::ToString() const
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

    Matrix4 Matrix4::CreateOrthoProjection(double left, double top, double right, double bottom, double near, double far)
    {
        Matrix4 projection;
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

    Matrix4 Matrix4::CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up)
    {
        Vector3 z = (eye - target).Normalized();
        Vector3 x = Vector3::CrossProduct(up, z).Normalized();
        Vector3 y = Vector3::CrossProduct(z, x).Normalized();

        Matrix4 rot(x.X, y.X, z.X, 0.0, x.Y, y.Y, z.Y, 0.0, x.Z, y.Z, z.Z, 0.0, 0, 0, 0, 1.0);
        Matrix4 trans = CreateTranslation(Vector3(-eye.X, -eye.Y, -eye.Z));
        return trans * rot;
    }

    Matrix4 Matrix4::CreatePerspectiveFieldOfView(double fovy, double aspect, double zNear, double zFar)
    {
        if (IsLessThanOrEqual(fovy, 0) || IsGreaterThan(fovy, VOXIUM_DOUBLE_PI))
            throw std::out_of_range("fovy");
        if (IsLessThanOrEqual(aspect, 0))
            throw std::out_of_range("aspect");
        if (IsLessThanOrEqual(zNear, 0))
            throw std::out_of_range("zNear");
        if (IsLessThanOrEqual(zFar, 0))
            throw std::out_of_range("zFar");
        if (IsGreaterThanOrEqual(zNear, zFar))
            throw std::out_of_range("zNear");

        double yMax = zNear * std::tan(0.5 * fovy);
        double yMin = -yMax;
        double xMin = yMin * aspect;
        double xMax = yMax * aspect;
        return CreatePerspectiveOffCenter(xMin, xMax, yMin, yMax, zNear, zFar);
    }

    Matrix4 Matrix4::CreatePerspectiveOffCenter(double left, double right, double bottom, double top, double zNear, double zFar)
    {
        if (IsLessThanOrEqual(zNear, 0))
            throw std::out_of_range("zNear");
        if (IsLessThanOrEqual(zFar, 0))
            throw std::out_of_range("zFar");
        if (IsGreaterThanOrEqual(zNear, zFar))
            throw std::out_of_range("zNear");

        double x = (2.0 * zNear) / (right - left);
        double y = (2.0 * zNear) / (top - bottom);
        double a = (right + left) / (right - left);
        double b = (top + bottom) / (top - bottom);
        double c = -(zFar + zNear) / (zFar - zNear);
        double d = -(2.0 * zFar * zNear) / (zFar - zNear);
        return Matrix4(x, 0, 0, 0, 0, y, 0, 0, a, b, c, -1, 0, 0, d, 0);
    }

    Matrix4 Matrix4::CreateFPSView(Vector3 eye, double pitch, double yaw)
    {
        eye.X = -eye.X;
        eye.Y = -eye.Y;

        double cosPitch = std::cos(pitch);
        double sinPitch = std::sin(pitch);
        double cosYaw   = std::cos(-yaw);
        double sinYaw   = std::sin(-yaw);

        Vector3 xaxis {cosYaw, 0, -sinYaw};
        Vector3 yaxis {sinYaw * sinPitch, cosPitch, cosYaw * sinPitch};
        Vector3 zaxis {sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw};

        Matrix4 rot(xaxis.X, yaxis.X, zaxis.X, 0.0, xaxis.Y, yaxis.Y, zaxis.Y, 0.0, xaxis.Z, yaxis.Z, zaxis.Z, 0.0, 0, 0, 0, 1.0);
        Matrix4 trans = CreateTranslation(Vector3(-eye.X, -eye.Y, -eye.Z));
        return trans * rot;
    }

    Matrix4 Matrix4::CreateRotationX(double rads)
    {
        double cosVal = std::cos(rads);
        double sinVal = std::sin(rads);
        return Matrix4(1, 0, 0, 0, 0, cosVal, sinVal, 0, 0, -sinVal, cosVal, 0, 0, 0, 0, 1);
    }

    Matrix4 Matrix4::CreateRotationY(double rads)
    {
        double cosVal = std::cos(rads);
        double sinVal = std::sin(rads);
        return Matrix4(cosVal, 0, -sinVal, 0, 0, 1, 0, 0, sinVal, 0, cosVal, 0, 0, 0, 0, 1);
    }

    Matrix4 Matrix4::CreateRotationZ(double rads)
    {
        double cosVal = std::cos(rads);
        double sinVal = std::sin(rads);
        return Matrix4(cosVal, sinVal, 0, 0, -sinVal, cosVal, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    }

    Matrix4 Matrix4::CreateRotationXY(double vX, double vY)
    {
        Matrix4 m  = Identity;
        double  sX = std::sin(vX);
        double  cX = std::cos(vX);
        double  sY = std::sin(vY);
        double  cY = std::cos(vY);
        m.M11      = cY;
        m.M12      = 0;
        m.M13      = -sY;

        m.M21 = sX * sY;
        m.M22 = cX;
        m.M23 = sX * cY;

        m.M31 = sY * cX;
        m.M32 = -sX;
        m.M33 = cX * cY;
        return m;
    }

    Matrix4 Matrix4::CreateRotationXZ(double vX, double vZ)
    {
        Matrix4 m  = Identity;
        double  sX = std::sin(vX);
        double  cX = std::cos(vX);
        double  sZ = std::sin(vZ);
        double  cZ = std::cos(vZ);
        m.M11      = cZ;
        m.M12      = sZ;

        m.M21 = -cX * sZ;
        m.M22 = cX * cZ;
        m.M23 = sX;

        m.M31 = sX * sZ;
        m.M32 = -sX * cZ;
        m.M33 = cX;
        return m;
    }

    Matrix4 Matrix4::CreateRotationYZ(double vY, double vZ)
    {
        Matrix4 m  = Identity;
        double  sY = std::sin(vY);
        double  cY = std::cos(vY);
        double  sZ = std::sin(vZ);
        double  cZ = std::cos(vZ);
        m.M11      = cY * cZ;
        m.M12      = cY * sZ;
        m.M13      = -sY;

        m.M21 = -sZ;
        m.M22 = cZ;
        m.M23 = 0;

        m.M31 = sY * cZ;
        m.M32 = sY * sZ;
        m.M33 = cY;
        return m;
    }

    Matrix4 Matrix4::CreateTranslation(double x, double y, double z) { return Matrix4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, x, y, z, 1); }

    Matrix4 Matrix4::CreateTranslation(const Vector3& vec) { return CreateTranslation(vec.X, vec.Y, vec.Z); }

    Matrix4 Matrix4::CreateScale(double scaleX, double scaleY, double scaleZ) { return Matrix4(scaleX, 0, 0, 0, 0, scaleY, 0, 0, 0, 0, scaleZ, 0, 0, 0, 0, 1); }

    Matrix4 Matrix4::CreateScale(double scaleFactor) { return CreateScale(scaleFactor, scaleFactor, scaleFactor); }

} // namespace Voxium::Core
