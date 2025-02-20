#pragma once

#include <cmath>
#include <string>
#include <format>
#include <limits>

namespace Voxium::Core {

    #pragma pack(push, 1)
    struct Vector {
        double X;
        double Y;

        constexpr Vector(double x, double y) : X(x), Y(y) {}

        constexpr Vector(double value) : X(value), Y(value) {}

        constexpr Vector() : X(0), Y(0) {}

        static const Vector Zero;

        constexpr int GetHashCode() {}

        static double Distance(const Vector& a, const Vector& b) {
            return (a - b).Magnitude();
        }

        double DotProduct(const Vector& other) const {
            return X * other.X + Y * other.Y;
        }

        double Magnitude() const {
            return std::sqrt(X * X + Y * Y);
        }

        Vector Normal() const {
            return Vector(Y, -X);
        }

        Vector Scale(double scale) const {
            return Vector(X * scale, Y * scale);
        }

        void Normalize() {
            double mag = Magnitude();
            if (mag != 0) {
                X /= mag;
                Y /= mag;
            }
        }

        Vector Normalized() const {
            double mag = Magnitude();
            return (mag != 0) ? Vector(X / mag, Y / mag) : *this;
        }

        // 静态方法：将点 point 绕 center 旋转 angle（单位：弧度）
        static Vector Rotate(const Vector& point, const Vector& center, double angle) {
            Vector delta = point - center;
            double startAngle = std::atan2(delta.Y, delta.X);
            double newAngle = startAngle + angle;
            double mag = delta.Magnitude();
            return Vector(center.X + std::cos(newAngle) * mag,
                          center.Y + std::sin(newAngle) * mag);
        }

        // 静态方法：返回三个点构成的两个向量的点积
        // 计算 (pointB - pointA) 与 (pointC - pointB) 的点积
        static double DotProduct(const Vector& pointA, const Vector& pointB, const Vector& pointC) {
            Vector AB = pointB - pointA;
            Vector BC = pointC - pointB;
            return AB.X * BC.X + AB.Y * BC.Y;
        }

        // 静态方法：返回三个点构成的两个向量的叉积
        // 计算 (pointB - pointA) 与 (pointC - pointA) 的叉积（结果为标量）
        static double CrossProduct(const Vector& pointA, const Vector& pointB, const Vector& pointC) {
            Vector AB = pointB - pointA;
            Vector AC = pointC - pointA;
            return AB.X * AC.Y - AB.Y * AC.X;
        }

        bool IsEqual(const Vector& other) const {
            constexpr double epsilon = std::numeric_limits<double>::epsilon();
            return std::abs(X - other.X) < epsilon && std::abs(Y - other.Y) < epsilon;
        }

        bool operator==(const Vector& other) const {
            return IsEqual(other);
        }
        bool operator!=(const Vector& other) const {
            return !(*this == other);
        }

        std::string ToString() const {
            return std::format("[{:.2f},{:.2f}]", X, Y);
        }

        friend Vector operator+(const Vector& a, const Vector& b) {
            return Vector(a.X + b.X, a.Y + b.Y);
        }

        friend Vector operator-(const Vector& a, const Vector& b) {
            return Vector(a.X - b.X, a.Y - b.Y);
        }

        friend Vector operator-(const Vector& a) {
            return Vector(-a.X, -a.Y);
        }

        friend Vector operator*(const Vector& vec, double scale) {
            return Vector(vec.X * scale, vec.Y * scale);
        }

        friend Vector operator/(const Vector& vec, double scale) {
            return Vector(vec.X / scale, vec.Y / scale);
        }
    };
    #pragma pack(pop)

    inline const Vector Vector::Zero = Vector(0.0, 0.0);

}
