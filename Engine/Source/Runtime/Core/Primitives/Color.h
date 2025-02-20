#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>

#include "Core/Base/Hash.h"

namespace Voxium::Core
{

    // 辅助函数：在两个字节之间线性插值
    constexpr uint8_t LerpByte(uint8_t a, uint8_t b, double pos) { return static_cast<uint8_t>(a + (b - a) * pos); }

    struct Color
    {
        uint32_t Value;

        // 构造函数：直接使用 ABGR 格式的 32 位整数
        constexpr Color(uint32_t abgr) : Value(abgr) {}

        // 构造函数：传入字节形式的红、绿、蓝和可选的 alpha（默认 255）
        constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) :
            Value(static_cast<uint32_t>(red) | (static_cast<uint32_t>(green) << 8) |
                  (static_cast<uint32_t>(blue) << 16) | (static_cast<uint32_t>(alpha) << 24))
        {}

        // 构造函数：传入浮点数形式的红、绿、蓝和可选 alpha（范围 0～1）
        constexpr Color(float red, float green, float blue, float alpha = 1.0f) :
            Color(static_cast<uint8_t>(red * 255.0f),
                  static_cast<uint8_t>(green * 255.0f),
                  static_cast<uint8_t>(blue * 255.0f),
                  static_cast<uint8_t>(alpha * 255.0f))
        {}

        // 构造函数：用现有 Color 和新的 alpha（字节形式）构造新颜色
        constexpr Color(const Color& other, uint8_t alpha) : Value(other.Value)
        {
            Value = (other.Value & 0x00FFFFFFu) | (static_cast<uint32_t>(alpha) << 24);
        }

        // 构造函数：用现有 Color 和新的 alpha（浮点形式）构造新颜色
        constexpr Color(const Color& other, float alpha) : Color(other, static_cast<uint8_t>(255 * alpha)) {}

        // 属性：返回 ABGR 格式（直接返回 Value）
        constexpr uint32_t ABGR() const { return Value; }

        // 属性：返回 ARGB 格式，交换红蓝分量
        constexpr uint32_t ARGB() const
        {
            return ((Value & 0xFFu) << 16) | (Value & 0xFF00FF00u) | ((Value & 0x00FF0000u) >> 16);
        }

        // Getter/Setter：红色分量
        constexpr uint8_t R() const { return static_cast<uint8_t>((Value >> 0) & 0xFF); }
        void              setR(uint8_t r) { Value = (Value & 0xFFFFFF00u) | r; }

        // Getter/Setter：绿色分量
        constexpr uint8_t G() const { return static_cast<uint8_t>((Value >> 8) & 0xFF); }
        void              setG(uint8_t g) { Value = (Value & 0xFFFF00FFu) | (static_cast<uint32_t>(g) << 8); }

        // Getter/Setter：蓝色分量
        constexpr uint8_t B() const { return static_cast<uint8_t>((Value >> 16) & 0xFF); }
        void              setB(uint8_t b) { Value = (Value & 0xFF00FFFFu) | (static_cast<uint32_t>(b) << 16); }

        // Getter/Setter：Alpha 分量
        constexpr uint8_t A() const { return static_cast<uint8_t>((Value >> 24) & 0xFF); }
        void              setA(uint8_t a) { Value = (Value & 0x00FFFFFFu) | (static_cast<uint32_t>(a) << 24); }

        // 将颜色转换为 Vector4F，分量归一化到 [0,1]
        constexpr Vector4F ToVector4F() const
        {
            return Vector4F(R() / 255.0f, G() / 255.0f, B() / 255.0f, A() / 255.0f);
        }

        // 将颜色转换为 Vector3F（忽略 Alpha）
        constexpr Vector3F ToVector3F() const { return Vector3F(R() / 255.0f, G() / 255.0f, B() / 255.0f); }

        // WithAlpha：返回一个新的颜色，其 alpha 由参数给定（以浮点形式，0～1）
        constexpr Color WithAlpha(float alpha) const { return Color(*this, alpha); }

        // 静态方法：颜色插值，pos 范围 [0,1]
        static constexpr Color Interpolate(const Color& A, const Color& B, double pos)
        {
            uint8_t r = LerpByte(A.R(), B.R(), pos);
            uint8_t g = LerpByte(A.G(), B.G(), pos);
            uint8_t b = LerpByte(A.B(), B.B(), pos);
            uint8_t a = LerpByte(A.A(), B.A(), pos);
            return Color(r, g, b, a);
        }

        // ToString：返回格式化字符串，如 "#AARRGGBB"
        std::string ToString() const
        {
            char buffer[10];
            std::snprintf(buffer, sizeof(buffer), "#%02X%02X%02X%02X", A(), R(), G(), B());
            return std::string(buffer);
        }

        // 重载相等比较
        constexpr bool operator==(const Color& other) const { return Value == other.Value; }
        constexpr bool operator!=(const Color& other) const { return !(*this == other); }

        // 预定义颜色（注意：C++20 中可以用 inline static）
        inline static const Color Clear  = Color(0, 0, 0, 0);
        inline static const Color White  = Color(255, 255, 255, 255);
        inline static const Color Black  = Color(0, 0, 0, 255);
        inline static const Color Green  = Color(113, 255, 164);
        inline static const Color Cyan   = Color(27, 247, 255);
        inline static const Color Orange = Color(255, 127, 42);
        inline static const Color Red    = Color(255, 0, 0);
        inline static const Color Yellow = Color(255, 204, 0);
        inline static const Color Gray   = Color(128, 128, 128, 255);
    };

} // namespace Voxium::Core
