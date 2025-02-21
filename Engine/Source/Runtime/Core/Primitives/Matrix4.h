#include <cmath>
#include <stdexcept>
#include <sstream>
#include <string>
#include <limits>
#include <iostream>

// 辅助类型：Vector3
struct Vector3 {
    double X, Y, Z;
    constexpr Vector3(double x = 0, double y = 0, double z = 0) : X(x), Y(y), Z(z) {}

    constexpr Vector3 operator-(const Vector3& other) const {
        return Vector3(X - other.X, Y - other.Y, Z - other.Z);
    }
    constexpr Vector3 operator+(const Vector3& other) const {
        return Vector3(X + other.X, Y + other.Y, Z + other.Z);
    }
    constexpr Vector3 operator*(double s) const {
        return Vector3(X * s, Y * s, Z * s);
    }
    double magnitude() const {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }
    Vector3 normalized() const {
        double m = magnitude();
        if (m == 0)
            throw std::runtime_error("Cannot normalize zero vector");
        return Vector3(X / m, Y / m, Z / m);
    }
    static constexpr Vector3 CrossProduct(const Vector3& a, const Vector3& b) {
        return Vector3(
            a.Y * b.Z - a.Z * b.Y,
            a.Z * b.X - a.X * b.Z,
            a.X * b.Y - a.Y * b.X
        );
    }
    static constexpr double DotProduct(const Vector3& a, const Vector3& b) {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
    }
};

// 辅助类型：Vector4
struct Vector4 {
    double X, Y, Z, W;
    constexpr Vector4(double x = 0, double y = 0, double z = 0, double w = 0)
        : X(x), Y(y), Z(z), W(w) {}
};

// 辅助类型：Matrix4F（float版本的4x4矩阵）
struct Matrix4F {
    float M11, M12, M13, M14;
    float M21, M22, M23, M24;
    float M31, M32, M33, M34;
    float M41, M42, M43, M44;
};

// 主类型：Matrix4
struct Matrix4 {
    double M11, M12, M13, M14;
    double M21, M22, M23, M24;
    double M31, M32, M33, M34;
    double M41, M42, M43, M44;

    // 构造函数：用16个 double 初始化各元素
    constexpr Matrix4(double m11, double m12, double m13, double m14,
                      double m21, double m22, double m23, double m24,
                      double m31, double m32, double m33, double m34,
                      double m41, double m42, double m43, double m44)
         : M11(m11), M12(m12), M13(m13), M14(m14),
           M21(m21), M22(m22), M23(m23), M24(m24),
           M31(m31), M32(m32), M33(m33), M34(m34),
           M41(m41), M42(m42), M43(m43), M44(m44)
    {}

    // 默认构造函数（零矩阵）
    Matrix4()
         : M11(0), M12(0), M13(0), M14(0),
           M21(0), M22(0), M23(0), M24(0),
           M31(0), M32(0), M33(0), M34(0),
           M41(0), M42(0), M43(0), M44(0)
    {}

    // 将 double 版本转换为 float 版本
    Matrix4F ToFloat() const {
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

    // 求逆函数（若行列式为0则抛出异常）
    Matrix4 Inverse() const {
        Matrix4 inv(
           M22 * M33 * M44 -
           M22 * M34 * M43 -
           M32 * M23 * M44 +
           M32 * M24 * M43 +
           M42 * M23 * M34 -
           M42 * M24 * M33,

           -M12 * M33 * M44 +
           M12 * M34 * M43 +
           M32 * M13 * M44 -
           M32 * M14 * M43 -
           M42 * M13 * M34 +
           M42 * M14 * M33,

           M12 * M23 * M44 -
           M12 * M24 * M43 -
           M22 * M13 * M44 +
           M22 * M14 * M43 +
           M42 * M13 * M24 -
           M42 * M14 * M23,

           -M12 * M23 * M34 +
           M12 * M24 * M33 +
           M22 * M13 * M34 -
           M22 * M14 * M33 -
           M32 * M13 * M24 +
           M32 * M14 * M23,

           -M21 * M33 * M44 +
           M21 * M34 * M43 +
           M31 * M23 * M44 -
           M31 * M24 * M43 -
           M41 * M23 * M34 +
           M41 * M24 * M33,

           M11 * M33 * M44 -
           M11 * M34 * M43 -
           M31 * M13 * M44 +
           M31 * M14 * M43 +
           M41 * M13 * M34 -
           M41 * M14 * M33,

           -M11 * M23 * M44 +
           M11 * M24 * M43 +
           M21 * M13 * M44 -
           M21 * M14 * M43 -
           M41 * M13 * M24 +
           M41 * M14 * M23,

           M11 * M23 * M34 -
           M11 * M24 * M33 -
           M21 * M13 * M34 +
           M21 * M14 * M33 +
           M31 * M13 * M24 -
           M31 * M14 * M23,

           M21 * M32 * M44 -
           M21 * M34 * M42 -
           M31 * M22 * M44 +
           M31 * M24 * M42 +
           M41 * M22 * M34 -
           M41 * M24 * M32,

           -M11 * M32 * M44 +
           M11 * M34 * M42 +
           M31 * M12 * M44 -
           M31 * M14 * M42 -
           M41 * M12 * M34 +
           M41 * M14 * M32,

           M11 * M22 * M44 -
           M11 * M24 * M42 -
           M21 * M12 * M44 +
           M21 * M14 * M42 +
           M41 * M12 * M24 -
           M41 * M14 * M22,

           -M11 * M22 * M34 +
           M11 * M24 * M32 +
           M21 * M12 * M34 -
           M21 * M14 * M32 -
           M31 * M12 * M24 +
           M31 * M14 * M22,

           -M21 * M32 * M43 +
           M21 * M33 * M42 +
           M31 * M22 * M43 -
           M31 * M23 * M42 -
           M41 * M22 * M33 +
           M41 * M23 * M32,

           M11 * M32 * M43 -
           M11 * M33 * M42 -
           M31 * M12 * M43 +
           M31 * M13 * M42 +
           M41 * M12 * M33 -
           M41 * M13 * M32,

           -M11 * M22 * M43 +
           M11 * M23 * M42 +
           M21 * M12 * M43 -
           M21 * M13 * M42 -
           M41 * M12 * M23 +
           M41 * M13 * M22,

           M11 * M22 * M33 -
           M11 * M23 * M32 -
           M21 * M12 * M33 +
           M21 * M13 * M32 +
           M31 * M12 * M23 -
           M31 * M13 * M22
        );
        double det = M11 * inv.M11 + M12 * inv.M21 + M13 * inv.M31 + M14 * inv.M41;
        if (det == 0)
            throw std::runtime_error("No inverse matrix");
        det = 1.0 / det;
        inv.M11 *= det; inv.M12 *= det; inv.M13 *= det; inv.M14 *= det;
        inv.M21 *= det; inv.M22 *= det; inv.M23 *= det; inv.M24 *= det;
        inv.M31 *= det; inv.M32 *= det; inv.M33 *= det; inv.M34 *= det;
        inv.M41 *= det; inv.M42 *= det; inv.M43 *= det; inv.M44 *= det;
        return inv;
    }

    // 静态工厂方法——正交投影矩阵
    static Matrix4 CreateOrthoProjection(double left, double top, double right, double bottom, double near, double far) {
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

    // 静态工厂方法——观察矩阵（LookAt）
    static Matrix4 CreateLookAt(const Vector3& eye, const Vector3& target, const Vector3& up) {
         Vector3 z = (eye - target).normalized();
         Vector3 x = Vector3::CrossProduct(up, z).normalized();
         Vector3 y = Vector3::CrossProduct(z, x).normalized();

         Matrix4 rot(
             x.X, y.X, z.X, 0.0,
             x.Y, y.Y, z.Y, 0.0,
             x.Z, y.Z, z.Z, 0.0,
             0,   0,   0,   1.0
         );
         Matrix4 trans = CreateTranslation(Vector3(-eye.X, -eye.Y, -eye.Z));
         return trans * rot;
    }

    // 静态工厂方法——透视投影（视野、长宽比、近平面、远平面）
    static Matrix4 CreatePerspectiveFieldOfView(double fovy, double aspect, double zNear, double zFar) {
         if (fovy <= 0 || fovy > M_PI)
              throw std::out_of_range("fovy");
         if (aspect <= 0)
              throw std::out_of_range("aspect");
         if (zNear <= 0)
              throw std::out_of_range("zNear");
         if (zFar <= 0)
              throw std::out_of_range("zFar");
         if (zNear >= zFar)
              throw std::out_of_range("zNear");

         double yMax = zNear * std::tan(0.5 * fovy);
         double yMin = -yMax;
         double xMin = yMin * aspect;
         double xMax = yMax * aspect;
         return CreatePerspectiveOffCenter(xMin, xMax, yMin, yMax, zNear, zFar);
    }

    // 静态工厂方法——透视投影（偏移版）
    static Matrix4 CreatePerspectiveOffCenter(double left, double right, double bottom, double top, double zNear, double zFar) {
         if (zNear <= 0)
              throw std::out_of_range("zNear");
         if (zFar <= 0)
              throw std::out_of_range("zFar");
         if (zNear >= zFar)
              throw std::out_of_range("zNear");

         double x = (2.0 * zNear) / (right - left);
         double y = (2.0 * zNear) / (top - bottom);
         double a = (right + left) / (right - left);
         double b = (top + bottom) / (top - bottom);
         double c = -(zFar + zNear) / (zFar - zNear);
         double d = -(2.0 * zFar * zNear) / (zFar - zNear);
         return Matrix4(
             x, 0, 0, 0,
             0, y, 0, 0,
             a, b, c, -1,
             0, 0, d, 0
         );
    }

    // 静态工厂方法——FPS视角（注意：这里对 eye 分量取反，与 C# 中相同）
    static Matrix4 CreateFPSView(Vector3 eye, double pitch, double yaw) {
         eye.X = -eye.X;
         eye.Y = -eye.Y;

         double cosPitch = std::cos(pitch);
         double sinPitch = std::sin(pitch);
         double cosYaw = std::cos(-yaw);
         double sinYaw = std::sin(-yaw);

         Vector3 xaxis { cosYaw, 0, -sinYaw };
         Vector3 yaxis { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
         Vector3 zaxis { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

         Matrix4 rot(
             xaxis.X, yaxis.X, zaxis.X, 0.0,
             xaxis.Y, yaxis.Y, zaxis.Y, 0.0,
             xaxis.Z, yaxis.Z, zaxis.Z, 0.0,
             0,       0,       0,       1.0
         );
         Matrix4 trans = CreateTranslation(Vector3(-eye.X, -eye.Y, -eye.Z));
         return trans * rot;
    }

    // 静态工厂方法——平移矩阵
    static Matrix4 CreateTranslation(double x, double y, double z) {
         return Matrix4(
             1, 0, 0, 0,
             0, 1, 0, 0,
             0, 0, 1, 0,
             x, y, z, 1
         );
    }
    static Matrix4 CreateTranslation(const Vector3& vec) {
         return CreateTranslation(vec.X, vec.Y, vec.Z);
    }

    // 静态工厂方法——绕 X、Y、Z 轴旋转
    static Matrix4 CreateRotationX(double rads) {
         double cos_val = std::cos(rads);
         double sin_val = std::sin(rads);
         return Matrix4(
             1, 0, 0, 0,
             0, cos_val, sin_val, 0,
             0, -sin_val, cos_val, 0,
             0, 0, 0, 1
         );
    }
    static Matrix4 CreateRotationY(double rads) {
         double cos_val = std::cos(rads);
         double sin_val = std::sin(rads);
         return Matrix4(
             cos_val, 0, -sin_val, 0,
             0, 1, 0, 0,
             sin_val, 0, cos_val, 0,
             0, 0, 0, 1
         );
    }
    static Matrix4 CreateRotationZ(double rads) {
         double cos_val = std::cos(rads);
         double sin_val = std::sin(rads);
         return Matrix4(
             cos_val, sin_val, 0, 0,
             -sin_val, cos_val, 0, 0,
             0, 0, 1, 0,
             0, 0, 0, 1
         );
    }

    // 静态工厂方法——组合旋转（XY、XZ、YZ）
    static Matrix4 CreateRotationXY(double vX, double vY) {
         Matrix4 m = Identity;
         double sX = std::sin(vX);
         double cX = std::cos(vX);
         double sY = std::sin(vY);
         double cY = std::cos(vY);
         m.M11 = cY;
         m.M12 = 0;
         m.M13 = -sY;

         m.M21 = sX * sY;
         m.M22 = cX;
         m.M23 = sX * cY;

         m.M31 = sY * cX;
         m.M32 = -sX;
         m.M33 = cX * cY;
         return m;
    }
    static Matrix4 CreateRotationXZ(double vX, double vZ) {
         Matrix4 m = Identity;
         double sX = std::sin(vX);
         double cX = std::cos(vX);
         double sZ = std::sin(vZ);
         double cZ = std::cos(vZ);
         m.M11 = cZ;
         m.M12 = sZ;

         m.M21 = -cX * sZ;
         m.M22 = cX * cZ;
         m.M23 = sX;

         m.M31 = sX * sZ;
         m.M32 = -sX * cZ;
         m.M33 = cX;
         return m;
    }
    static Matrix4 CreateRotationYZ(double vY, double vZ) {
         Matrix4 m = Identity;
         double sY = std::sin(vY);
         double cY = std::cos(vY);
         double sZ = std::sin(vZ);
         double cZ = std::cos(vZ);
         m.M11 = cY * cZ;
         m.M12 = cY * sZ;
         m.M13 = -sY;

         m.M21 = -sZ;
         m.M22 = cZ;
         m.M23 = 0;

         m.M31 = sY * cZ;
         m.M32 = sY * sZ;
         m.M33 = cY;
         return m;
    }

    // 静态工厂方法——缩放矩阵
    static Matrix4 CreateScale(double scaleX, double scaleY, double scaleZ) {
         return Matrix4(
             scaleX, 0, 0, 0,
             0, scaleY, 0, 0,
             0, 0, scaleZ, 0,
             0, 0, 0, 1
         );
    }
    static Matrix4 CreateScale(double scaleFactor) {
         return CreateScale(scaleFactor, scaleFactor, scaleFactor);
    }

    // 转置
    Matrix4 Transpose() const {
         return Matrix4(
             M11, M21, M31, M41,
             M12, M22, M32, M42,
             M13, M23, M33, M43,
             M14, M24, M34, M44
         );
    }

    // 矩阵乘法运算符
    Matrix4 operator*(const Matrix4& b) const {
         return Matrix4(
             M11 * b.M11 + M12 * b.M21 + M13 * b.M31 + M14 * b.M41,
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
             M41 * b.M14 + M42 * b.M24 + M43 * b.M34 + M44 * b.M44
         );
    }

    // 判断矩阵各元素是否全等
    bool IsEqual(const Matrix4& other) const {
         return M11 == other.M11 &&
                M12 == other.M12 &&
                M13 == other.M13 &&
                M14 == other.M14 &&
                M21 == other.M21 &&
                M22 == other.M22 &&
                M23 == other.M23 &&
                M24 == other.M24 &&
                M31 == other.M31 &&
                M32 == other.M32 &&
                M33 == other.M33 &&
                M34 == other.M34 &&
                M41 == other.M41 &&
                M42 == other.M42 &&
                M43 == other.M43 &&
                M44 == other.M44;
    }
    bool operator==(const Matrix4& other) const {
         return IsEqual(other);
    }
    bool operator!=(const Matrix4& other) const {
         return !IsEqual(other);
    }

    // 类似 ToString 方法（返回 "[ (row1), (row2), (row3), (row4) ]" 格式）
    std::string to_string() const {
         std::ostringstream oss;
         oss << "[(" << M11 << ", " << M12 << ", " << M13 << ", " << M14 << "), "
             << "(" << M21 << ", " << M22 << ", " << M23 << ", " << M24 << "), "
             << "(" << M31 << ", " << M32 << ", " << M33 << ", " << M34 << "), "
             << "(" << M41 << ", " << M42 << ", " << M43 << ", " << M44 << ")]";
         return oss.str();
    }

    // 静态成员——单位矩阵
    inline static const Matrix4 Identity = Matrix4(1, 0, 0, 0,
                                                     0, 1, 0, 0,
                                                     0, 0, 1, 0,
                                                     0, 0, 0, 1);
};
