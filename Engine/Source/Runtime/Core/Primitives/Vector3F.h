#pragma once

#include <cmath>
#include <sstream>
#include <string>
#include <iomanip>
#include <functional>
#include <iostream>

namespace Voxium::Core {

struct Vector3F {
    float X, Y, Z;

    static constexpr Vector3F Zero;
    static constexpr Vector3F Forward;
    static constexpr Vector3F Up;

    constexpr Vector3F(float x) : X(x), Y(x), Z(x) {}
    constexpr Vector3F(float x, float y, float z) : X(x), Y(y), Z(z) {}
    constexpr Vector3F(const Int3& v) : X(static_cast<float>(v.X)),
                                        Y(static_cast<float>(v.Y)),
                                        Z(static_cast<float>(v.Z)) {}
    constexpr Vector3F(const Vector4F& v) : X(v.X), Y(v.Y), Z(v.Z) {}

    constexpr VectorF XY() const { return VectorF(X, Y); }
    constexpr VectorF XZ() const { return VectorF(X, Z); }
    constexpr Vector XZD() const { return Vector(static_cast<double>(X), static_cast<double>(Z)); }
    constexpr Vector3 D() const { return Vector3(static_cast<double>(X), static_cast<double>(Y), static_cast<double>(Z)); }
    constexpr Vector3F XO3() const { return Vector3F(X, 0.0f, Z); }
    constexpr Vector4F XYZW() const { return Vector4F(X, Y, Z, 1.0f); }

    float Magnitude() const {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }

    void Normalize() {
        float mag = Magnitude();
        if (mag != 0.0f) {
            float scale = 1.0f / mag;
            X *= scale;
            Y *= scale;
            Z *= scale;
        }
    }

    constexpr Vector3F Normalized() const {
        float mag = Magnitude();
        // 避免除零，零向量返回自身
        if (mag == 0.0f) return Vector3F(0.0f);
        float scale = 1.0f / mag;
        return Vector3F(X * scale, Y * scale, Z * scale);
    }

    // 静态数学函数
    static constexpr float DotProduct(const Vector3F& a, const Vector3F& b) {
        return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
    }

    static constexpr Vector3F CrossProduct(const Vector3F& a, const Vector3F& b) {
        return Vector3F(a.Y * b.Z - a.Z * b.Y,
                        a.Z * b.X - a.X * b.Z,
                        a.X * b.Y - a.Y * b.X);
    }

    static Vector3F FromPitchYaw(float pitch, float yaw) {
        float cosPitch = std::cos(pitch);
        return Vector3F(cosPitch * std::sin(yaw),
                        std::sin(pitch),
                        cosPitch * std::cos(yaw));
    }

    // 重写 ToString（转换为字符串）
    std::string to_string() const {
        std::ostringstream oss;
        oss << "[" << std::fixed << std::setprecision(2) << X << ","
            << std::fixed << std::setprecision(2) << Y << ","
            << std::fixed << std::setprecision(2) << Z << "]";
        return oss.str();
    }

    // GetHashCode 的简易实现（利用 std::hash）
    std::size_t GetHashCode() const {
        std::size_t h1 = std::hash<float>{}(X);
        std::size_t h2 = std::hash<float>{}(Y);
        std::size_t h3 = std::hash<float>{}(Z);
        return h1 ^ (h2 << 1) ^ (h3 << 2);
    }

    // Equals / IsEqual
    bool Equals(const Vector3F& other) const {
        return X == other.X && Y == other.Y && Z == other.Z;
    }
    bool IsEqual(const Vector3F& other) const {
        return Equals(other);
    }

    // 操作符重载
    friend constexpr bool operator==(const Vector3F& a, const Vector3F& b) {
        return a.Equals(b);
    }
    friend constexpr bool operator!=(const Vector3F& a, const Vector3F& b) {
        return !a.Equals(b);
    }
    friend constexpr Vector3F operator+(const Vector3F& a, const Vector3F& b) {
        return Vector3F(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
    }
    friend constexpr Vector3F operator-(const Vector3F& a, const Vector3F& b) {
        return Vector3F(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
    }
    friend constexpr Vector3F operator*(const Vector3F& a, const Vector3F& b) {
        return Vector3F(a.X * b.X, a.Y * b.Y, a.Z * b.Z);
    }
    friend constexpr Vector3F operator/(const Vector3F& a, const Vector3F& b) {
        return Vector3F(a.X / b.X, a.Y / b.Y, a.Z / b.Z);
    }
    friend constexpr Vector3F operator*(const Vector3F& a, float b) {
        return Vector3F(a.X * b, a.Y * b, a.Z * b);
    }
    friend constexpr Vector3F operator/(const Vector3F& a, float b) {
        return Vector3F(a.X / b, a.Y / b, a.Z / b);
    }
    friend constexpr Vector3F operator-(const Vector3F& a) {
        return Vector3F(-a.X, -a.Y, -a.Z);
    }

    // 与矩阵相乘（按照 C# 代码约定，视 Vector3F 为行向量 (x,y,z,1)）
    friend Vector3F operator*(const Vector3F& a, const Matrix4& b) {
        float x = a.X * static_cast<float>(b.M11) + a.Y * static_cast<float>(b.M21) +
                  a.Z * static_cast<float>(b.M31) + 1.0f * static_cast<float>(b.M41);
        float y = a.X * static_cast<float>(b.M12) + a.Y * static_cast<float>(b.M22) +
                  a.Z * static_cast<float>(b.M32) + 1.0f * static_cast<float>(b.M42);
        float z = a.X * static_cast<float>(b.M13) + a.Y * static_cast<float>(b.M23) +
                  a.Z * static_cast<float>(b.M33) + 1.0f * static_cast<float>(b.M43);
        return Vector3F(x, y, z);
    }

    friend Vector3F operator*(const Vector3F& a, const Matrix4F& b) {
        float x = a.X * b.M11 + a.Y * b.M21 + a.Z * b.M31 + 1.0f * b.M41;
        float y = a.X * b.M12 + a.Y * b.M22 + a.Z * b.M32 + 1.0f * b.M42;
        float z = a.X * b.M13 + a.Y * b.M23 + a.Z * b.M33 + 1.0f * b.M43;
        return Vector3F(x, y, z);
    }
}
