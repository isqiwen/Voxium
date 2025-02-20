#pragma once

#include <cmath>
#include <string>
#include <format>
#include <limits>

namespace Voxium::Core {

    #pragma pack(push, 1)
    struct VectorF {
        float X;
        float Y;

        constexpr VectorF(float x, float y) : X(x), Y(y) {}

        constexpr VectorF(float value) : X(value), Y(value) {}

        constexpr VectorF() : X(0), Y(0) {}

        static const VectorF Zero;

        constexpr int GetHashCode() {}

        static float Distance(const VectorF& a, const VectorF& b) {
            return (a - b).Magnitude();
        }

        float DotProduct(const VectorF& other) const {
            return X * other.X + Y * other.Y;
        }

        float Magnitude() const {
            return std::sqrt(X * X + Y * Y);
        }

        VectorF Normal() const {
            return VectorF(Y, -X);
        }

        VectorF Scale(float scale) const {
            return VectorF(X * scale, Y * scale);
        }

        void Normalize() {
            float mag = Magnitude();
            if (mag != 0) {
                X /= mag;
                Y /= mag;
            }
        }

        VectorF Normalized() const {
            float mag = Magnitude();
            return (mag != 0) ? VectorF(X / mag, Y / mag) : *this;
        }

        static VectorF Rotate(const VectorF& point, const VectorF& center, float angle) {
            VectorF delta = point - center;
            float startAngle = std::atan2(delta.Y, delta.X);
            float newAngle = startAngle + angle;
            float mag = delta.Magnitude();
            return VectorF(center.X + std::cos(newAngle) * mag,
                          center.Y + std::sin(newAngle) * mag);
        }

        static float DotProduct(const VectorF& pointA, const VectorF& pointB, const VectorF& pointC) {
            VectorF AB = pointB - pointA;
            VectorF BC = pointC - pointB;
            return AB.X * BC.X + AB.Y * BC.Y;
        }

        static float CrossProduct(const VectorF& pointA, const VectorF& pointB, const VectorF& pointC) {
            VectorF AB = pointB - pointA;
            VectorF AC = pointC - pointA;
            return AB.X * AC.Y - AB.Y * AC.X;
        }

        bool IsEqual(const VectorF& other) const {
            constexpr float epsilon = std::numeric_limits<float>::epsilon();
            return std::abs(X - other.X) < epsilon && std::abs(Y - other.Y) < epsilon;
        }

        bool operator==(const VectorF& other) const {
            return IsEqual(other);
        }

        bool operator!=(const VectorF& other) const {
            return !(*this == other);
        }

        std::string ToString() const {
            return std::format("[{:.2f},{:.2f}]", X, Y);
        }

        friend VectorF operator+(const VectorF& a, const VectorF& b) {
            return VectorF(a.X + b.X, a.Y + b.Y);
        }

        friend VectorF operator-(const VectorF& a, const VectorF& b) {
            return VectorF(a.X - b.X, a.Y - b.Y);
        }

        friend VectorF operator-(const VectorF& a) {
            return VectorF(-a.X, -a.Y);
        }

        friend VectorF operator*(const VectorF& vec, float scale) {
            return VectorF(vec.X * scale, vec.Y * scale);
        }

        friend VectorF operator/(const VectorF& vec, float scale) {
            return VectorF(vec.X / scale, vec.Y / scale);
        }
    };
    #pragma pack(pop)

    inline const VectorF VectorF::Zero = VectorF(0.0, 0.0);

}
