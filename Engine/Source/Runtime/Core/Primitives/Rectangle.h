#pragma once

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>
#include <format>

#include "Gravity.h"
#include "Size.h"
#include "Vector.h"

namespace Voxium::Core
{
    #pragma pack(push, 1)
    struct Rectangle {
        double Left;
        double Top;
        double Width;
        double Height;

        // 静态 Empty 矩形（所有值为 0）
        static const Rectangle Empty;

        constexpr Rectangle(double left, double top, double width, double height)
            : Left(left), Top(top), Width(width), Height(height) {}

        constexpr Rectangle(const Vector& topleft, const Size& size)
            : Left(topleft.X), Top(topleft.Y), Width(size.Width), Height(size.Height) {}

        constexpr Rectangle() : Left(0), Top(0), Width(0), Height(0) {}

        constexpr bool IsEmpty() const {
            return Width == 0 || Height == 0;
        }

        constexpr double Bottom() const { return Top + Height; }

        void SetBottom(double bottom) { Height = bottom - Top; }

        constexpr double Right() const { return Left + Width; }

        void SetRight(double right) { Width = right - Left; }

        constexpr Vector Center() const {
            return Vector(CenterX(), CenterY());
        }

        constexpr Vector CenterSnapped() const {
            return Vector(std::floor(CenterX()), std::floor(CenterY()));
        }

        constexpr double CenterX() const { return (Left + Right()) / 2.0; }

        constexpr double CenterY() const { return (Top + Bottom()) / 2.0; }

        // Clip：返回与 outer 的交集矩形
        constexpr Rectangle Clip(const Rectangle& outer) const {
            double newLeft = std::max(Left, outer.Left);
            double newTop = std::max(Top, outer.Top);
            double newRight = std::min(Right(), outer.Right());
            double newBottom = std::min(Bottom(), outer.Bottom());
            return Rectangle(newLeft, newTop, newRight - newLeft, newBottom - newTop);
        }

        // ApplyInsets：内边距调整，重载两个版本
        constexpr Rectangle ApplyInsets(double h) const {
            return ApplyInsets(h, h, h, h);
        }

        constexpr Rectangle ApplyInsets(double leftInset, double topInset, double rightInset, double bottomInset) const {
            return Rectangle(Left + leftInset,
                             Top + topInset,
                             Width - leftInset - rightInset,
                             Height - topInset - bottomInset);
        }

        // ApplyOutsets：外边距调整，类似于内边距但取反
        constexpr Rectangle ApplyOutsets(double h) const {
            return ApplyOutsets(h, h, h, h);
        }

        constexpr Rectangle ApplyOutsets(double leftOutset, double topOutset, double rightOutset, double bottomOutset) const {
            return Rectangle(Left - leftOutset,
                             Top - topOutset,
                             Width + leftOutset + rightOutset,
                             Height + topOutset + bottomOutset);
        }

        // Inflate：扩大矩形（在四周增加宽、高）
        constexpr Rectangle Inflate(double w, double h) const {
            return Rectangle(Left - w, Top - h, Width + 2 * w, Height + 2 * h);
        }

        // ApplyGravity：将一个指定尺寸的矩形按对齐规则放置在当前矩形内
        constexpr Rectangle ApplyGravity(const Size& size, Gravity align) const {
            // 处理缩放选项
            Gravity scalePart = align & Gravity::ScaleMask;
            if (scalePart == Gravity::Fill) {
                return *this;
            } else if (scalePart == Gravity::AspectFill) {
                double scale = std::max(this->Size().Width / size.Width,
                                        this->Size().Height / size.Height);
                Size newSize = size * scale;
                Vector newTopLeft = Center() - (size.ToVector() * (scale / 2.0));
                return Rectangle(newTopLeft, newSize);
            } else if (scalePart == Gravity::AspectFit) {
                double scale = std::min(this->Size().Width / size.Width,
                                        this->Size().Height / size.Height);
                Size newSize = size * scale;
                Vector newTopLeft = Center() - (size.ToVector() * (scale / 2.0));
                return Rectangle(newTopLeft, newSize);
            }

            // 分别处理水平与垂直对齐
            double newLeft;
            Gravity horizontalPart = align & Gravity::HorizontalMask;
            if (horizontalPart == Gravity::Right) {
                newLeft = Right() - size.Width;
            } else if (horizontalPart == Gravity::CenterHorizontal) {
                newLeft = (Left + Right() - size.Width) / 2.0;
            } else {
                newLeft = Left;
            }

            double newTop;
            Gravity verticalPart = align & Gravity::VerticalMask;
            if (verticalPart == Gravity::Bottom) {
                newTop = Bottom() - size.Height;
            } else if (verticalPart == Gravity::CenterVertical) {
                newTop = (Top + Bottom() - size.Height) / 2.0;
            } else {
                newTop = Top;
            }

            return Rectangle(newLeft, newTop, size.Width, size.Height);
        }

        // Contains：判断一个点是否位于矩形内（左、上包含，右、下不包含）
        constexpr bool Contains(const Vector& point) const {
            return point.X >= Left && point.X < Right() &&
                   point.Y >= Top && point.Y < Bottom();
        }

        // TopLeft 属性：获取和设置矩形的左上角
        constexpr Vector TopLeft() const {
            return Vector(Left, Top);
        }

        void SetTopLeft(const Vector& v) {
            Left = v.X;
            Top = v.Y;
        }

        // TopRight 属性：获取和设置矩形的右上角
        constexpr Vector TopRight() const {
            return Vector(Right(), Top);
        }

        void SetTopRight(const Vector& v) {
            SetRight(v.X);
            Top = v.Y;
        }

        // BottomLeft 属性：只读，返回左下角
        constexpr Vector BottomLeft() const {
            return Vector(Left, Bottom());
        }

        // BottomRight 属性：获取和设置矩形的右下角
        constexpr Vector BottomRight() const {
            return Vector(Right(), Bottom());
        }

        void SetBottomRight(const Vector& v) {
            // 以 TopLeft 为锚点，设置 Size 为 v - TopLeft
            Size newSize = (v - TopLeft()).ToVector().ToSize();
            SetSize(newSize);
        }

        // Size 属性：获取和设置尺寸
        constexpr Size Size() const {
            return Size(Width, Height);
        }

        void SetSize(const Size& s) {
            Width = s.Width;
            Height = s.Height;
        }

        std::string ToString() const {
            return std::format("[{:.2f},{:.2f} {:.2f}x{:.2f}]", Left, Top, Width, Height);
        }

        // 重载相等运算符
        friend constexpr bool operator==(const Rectangle& a, const Rectangle& b) {
            return a.Left == b.Left && a.Top == b.Top &&
                   a.Width == b.Width && a.Height == b.Height;
        }

        friend constexpr bool operator!=(const Rectangle& a, const Rectangle& b) {
            return !(a == b);
        }

        // 运算符 * 与 /：对矩形进行缩放，按 TopLeft 和 Size 同时缩放
        friend constexpr Rectangle operator*(const Rectangle& rect, double scale) {
            return Rectangle(rect.TopLeft() * scale, rect.Size() * scale);
        }

        friend constexpr Rectangle operator/(const Rectangle& rect, double scale) {
            return Rectangle(rect.TopLeft() / scale, rect.Size() / scale);
        }
    };
    #pragma pack(pop)
}
