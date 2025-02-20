#pragma once

#include <cmath>
#include <format>
#include <stdexcept>
#include <string>

#include "Core/Base/Hash.h"

namespace Voxium::Core
{

    //--------------------------------
    // 辅助类型
    //--------------------------------

    // 整型三维向量，用于转换（简单实现）
    struct Int3
    {
        int X, Y, Z;
        constexpr Int3(int x, int y, int z) : X(x), Y(y), Z(z) {}
    };

    // 2D 向量（双精度）
    struct Vector
    {
        double X, Y;
        constexpr Vector(double x = 0.0, double y = 0.0) : X(x), Y(y) {}
        std::string ToString() const { return std::format("[{:.2f},{:.2f}]", X, Y); }
    };

    // 2D 向量（浮点型）
    struct VectorF
    {
        float X, Y;
        constexpr VectorF(float x = 0.f, float y = 0.f) : X(x), Y(y) {}
    };

    // 3D 向量（浮点型）
    struct Vector3F
    {
        float X, Y, Z;
        constexpr Vector3F(float x = 0.f, float y = 0.f, float z = 0.f) : X(x), Y(y), Z(z) {}
    };

    // 4D 向量（双精度）
    struct Vector4
    {
        double X, Y, Z, W;
        constexpr Vector4(double x = 0.0, double y = 0.0, double z = 0.0, double w = 0.0) : X(x), Y(y), Z(z), W(w) {}
    };

    // 4D 向量（浮点型）
    struct Vector4F
    {
        float X, Y, Z, W;
        constexpr Vector4F(float x = 0.f, float y = 0.f, float z = 0.f, float w = 0.f) : X(x), Y(y), Z(z), W(w) {}
    };

    // Minimal Matrix4 (双精度) 用于变换
    struct Matrix4
    {
        double M11, M12, M13, M14;
        double M21, M22, M23, M24;
        double M31, M32, M33, M34;
        double M41, M42, M43, M44;
    };

    // Minimal Matrix4F (浮点型) 用于变换
    struct Matrix4F
    {
        float M11, M12, M13, M14;
        float M21, M22, M23, M24;
        float M31, M32, M33, M34;
        float M41, M42, M43, M44;
    };

    //--------------------------------
    // Vector3 实现
    //--------------------------------
    struct Vector3
    {
        double X;
        double Y;
        double Z;

        // 预定义零向量
        inline static const Vector3 Zero {0.0, 0.0, 0.0};

        // 构造函数：所有分量相同
        constexpr Vector3(double x) : X(x), Y(x), Z(x) {}

        // 构造函数：各分量分别指定
        constexpr Vector3(double x, double y, double z) : X(x), Y(y), Z(z) {}

        // 从 Int3 构造
        constexpr Vector3(const Int3& v) : X(v.X), Y(v.Y), Z(v.Z) {}

        // 静态方法：从 pitch 和 yaw 构造向量
        static Vector3 FromPitchYaw(double pitch, double yaw)
        {
            double cosPitch = std::cos(pitch);
            return Vector3(cosPitch * std::sin(yaw), std::sin(pitch), cosPitch * std::cos(yaw));
        }

        // 转换属性：
        // 返回 (X, Y) 2D 向量
        constexpr Vector XY() const { return Vector(X, Y); }
        // 返回 (X, Z) 2D 向量
        constexpr Vector XZ() const { return Vector(X, Z); }
        // 返回 (X, Z) 2D 浮点向量
        constexpr VectorF XZF() const { return VectorF(static_cast<float>(X), static_cast<float>(Z)); }
        // 返回 3D 浮点向量
        constexpr Vector3F F() const
        {
            return Vector3F(static_cast<float>(X), static_cast<float>(Y), static_cast<float>(Z));
        }
        // 返回向量 (X, 0, Z)
        constexpr Vector3 XOZ() const { return Vector3(X, 0.0, Z); }
        // 返回齐次坐标向量 (X, Y, Z, 1)
        constexpr Vector4 XYZW() const { return Vector4(X, Y, Z, 1.0); }

        // 数学运算
        constexpr double Magnitude() const { return std::sqrt(X * X + Y * Y + Z * Z); }
        void             Normalize()
        {
            double mag = Magnitude();
            if (mag == 0)
                throw std::runtime_error("Cannot normalize zero vector");
            double scale = 1.0 / mag;
            X *= scale;
            Y *= scale;
            Z *= scale;
        }
        constexpr Vector3 Normalized() const
        {
            double mag = Magnitude();
            if (mag == 0)
                return *this; // 或者抛出异常
            double scale = 1.0 / mag;
            return Vector3(X * scale, Y * scale, Z * scale);
        }

        // 静态数学函数
        static constexpr double DotProduct(const Vector3& a, const Vector3& b)
        {
            return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
        }
        static constexpr Vector3 CrossProduct(const Vector3& a, const Vector3& b)
        {
            return Vector3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
        }

        // ToString 使用 std::format 返回格式化字符串
        std::string ToString() const { return std::format("[{:.2f},{:.2f},{:.2f}]", X, Y, Z); }

        // 判断相等（逐分量比较）
        constexpr bool IsEqual(const Vector3& other) const { return X == other.X && Y == other.Y && Z == other.Z; }

        // 运算符重载，均定义为友元函数
        friend constexpr bool    operator==(const Vector3& a, const Vector3& b) { return a.IsEqual(b); }
        friend constexpr bool    operator!=(const Vector3& a, const Vector3& b) { return !a.IsEqual(b); }
        friend constexpr Vector3 operator+(const Vector3& a, const Vector3& b)
        {
            return Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }
        friend constexpr Vector3 operator-(const Vector3& a, const Vector3& b)
        {
            return Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }
        friend constexpr Vector3 operator*(const Vector3& a, const Vector3& b)
        {
            return Vector3(a.X * b.X, a.Y * b.Y, a.Z * b.Z);
        }
        friend constexpr Vector3 operator/(const Vector3& a, const Vector3& b)
        {
            return Vector3(a.X / b.X, a.Y / b.Y, a.Z / b.Z);
        }
        friend constexpr Vector3 operator*(const Vector3& a, double b) { return Vector3(a.X * b, a.Y * b, a.Z * b); }
        friend constexpr Vector3 operator/(const Vector3& a, double b) { return Vector3(a.X / b, a.Y / b, a.Z / b); }
        friend constexpr Vector3 operator-(const Vector3& a) { return Vector3(-a.X, -a.Y, -a.Z); }

        // 支持与矩阵相乘（假设矩阵代表 4x4 变换矩阵）
        friend Vector3 operator*(const Vector3& a, const Matrix4& b)
        {
            // 计算 a*(b)，视 a 为齐次坐标 (X,Y,Z,1)
            return Vector3(a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + 1.0 * b.M41,
                           a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + 1.0 * b.M42,
                           a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + 1.0 * b.M43);
        }
        friend Vector3 operator*(const Vector3& a, const Matrix4F& b)
        {
            return Vector3(a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + 1.0f * b.M41,
                           a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + 1.0f * b.M42,
                           a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + 1.0f * b.M43);
        }
    };

} // namespace Voxium::Core
