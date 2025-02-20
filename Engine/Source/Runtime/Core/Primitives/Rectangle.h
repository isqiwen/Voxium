#pragma once

#include <algorithm>
#include <cmath>
#include <format>
#include <stdexcept>
#include <string>

#include "Core/Base/Hash.h"
#include "Gravity.h"
#include "Size.h"
#include "Vector.h"

namespace Voxium::Core
{
#pragma pack(push, 1)
    struct Rectangle
    {
        double Left;
        double Top;
        double Width;
        double Height;

        static const Rectangle Empty;

        constexpr Rectangle(double left, double top, double width, double height) :
            Left(left), Top(top), Width(width), Height(height)
        {}

        constexpr Rectangle(const Vector& topleft, const Size& size) :
            Left(topleft.X), Top(topleft.Y), Width(size.Width), Height(size.Height)
        {}

        constexpr Rectangle() : Left(0), Top(0), Width(0), Height(0) {}

        constexpr bool IsEmpty() const { return Width == 0 || Height == 0; }

        constexpr double Bottom() const { return Top + Height; }

        void SetBottom(double bottom) { Height = bottom - Top; }

        constexpr double Right() const { return Left + Width; }

        void SetRight(double right) { Width = right - Left; }

        constexpr Vector Center() const { return Vector(CenterX(), CenterY()); }

        constexpr Vector CenterSnapped() const { return Vector(std::floor(CenterX()), std::floor(CenterY())); }

        constexpr double CenterX() const { return (Left + Right()) / 2.0; }

        constexpr double CenterY() const { return (Top + Bottom()) / 2.0; }

        constexpr Rectangle Clip(const Rectangle& outer) const
        {
            double newLeft   = std::max(Left, outer.Left);
            double newTop    = std::max(Top, outer.Top);
            double newRight  = std::min(Right(), outer.Right());
            double newBottom = std::min(Bottom(), outer.Bottom());
            return Rectangle(newLeft, newTop, newRight - newLeft, newBottom - newTop);
        }

        constexpr Rectangle ApplyInsets(double h) const { return ApplyInsets(h, h, h, h); }

        constexpr Rectangle ApplyInsets(double leftInset, double topInset, double rightInset, double bottomInset) const
        {
            return Rectangle(
                Left + leftInset, Top + topInset, Width - leftInset - rightInset, Height - topInset - bottomInset);
        }

        constexpr Rectangle ApplyOutsets(double h) const { return ApplyOutsets(h, h, h, h); }

        constexpr Rectangle
        ApplyOutsets(double leftOutset, double topOutset, double rightOutset, double bottomOutset) const
        {
            return Rectangle(Left - leftOutset,
                             Top - topOutset,
                             Width + leftOutset + rightOutset,
                             Height + topOutset + bottomOutset);
        }

        constexpr Rectangle Inflate(double w, double h) const
        {
            return Rectangle(Left - w, Top - h, Width + 2 * w, Height + 2 * h);
        }

        constexpr Rectangle ApplyGravity(const Size& size, Gravity align) const
        {
            Gravity scalePart = align & Gravity::ScaleMask;
            if (scalePart == Gravity::Fill)
            {
                return *this;
            }
            else if (scalePart == Gravity::AspectFill)
            {
                double             scale = std::max(this->Size().Width / size.Width, this->Size().Height / size.Height);
                Voxium::Core::Size newSize    = size * scale;
                Vector             newTopLeft = Center() - (size.ToVector() * (scale / 2.0));
                return Rectangle(newTopLeft, newSize);
            }
            else if (scalePart == Gravity::AspectFit)
            {
                double             scale = std::min(this->Size().Width / size.Width, this->Size().Height / size.Height);
                Voxium::Core::Size newSize    = size * scale;
                Vector             newTopLeft = Center() - (size.ToVector() * (scale / 2.0));
                return Rectangle(newTopLeft, newSize);
            }

            double  newLeft;
            Gravity horizontalPart = align & Gravity::HorizontalMask;
            if (horizontalPart == Gravity::Right)
            {
                newLeft = Right() - size.Width;
            }
            else if (horizontalPart == Gravity::CenterHorizontal)
            {
                newLeft = (Left + Right() - size.Width) / 2.0;
            }
            else
            {
                newLeft = Left;
            }

            double  newTop;
            Gravity verticalPart = align & Gravity::VerticalMask;
            if (verticalPart == Gravity::Bottom)
            {
                newTop = Bottom() - size.Height;
            }
            else if (verticalPart == Gravity::CenterVertical)
            {
                newTop = (Top + Bottom() - size.Height) / 2.0;
            }
            else
            {
                newTop = Top;
            }

            return Rectangle(newLeft, newTop, size.Width, size.Height);
        }

        constexpr bool Contains(const Vector& point) const
        {
            return point.X >= Left && point.X < Right() && point.Y >= Top && point.Y < Bottom();
        }

        constexpr Vector TopLeft() const { return Vector(Left, Top); }

        void SetTopLeft(const Vector& v)
        {
            Left = v.X;
            Top  = v.Y;
        }

        constexpr Vector TopRight() const { return Vector(Right(), Top); }

        void SetTopRight(const Vector& v)
        {
            SetRight(v.X);
            Top = v.Y;
        }

        constexpr Vector BottomLeft() const { return Vector(Left, Bottom()); }

        constexpr Vector BottomRight() const { return Vector(Right(), Bottom()); }

        void SetBottomRight(const Vector& v)
        {
            Vector             newVector = v - TopLeft();
            Voxium::Core::Size newSize(newVector.X, newVector.Y);
            SetSize(newSize);
        }

        constexpr Voxium::Core::Size Size() const { return Voxium::Core::Size(Width, Height); }

        void SetSize(const Voxium::Core::Size& s)
        {
            Width  = s.Width;
            Height = s.Height;
        }

        std::string ToString() const { return std::format("[{:.2f},{:.2f} {:.2f}x{:.2f}]", Left, Top, Width, Height); }

        friend bool operator==(const Rectangle& a, const Rectangle& b)
        {
            constexpr double epsilon = std::numeric_limits<double>::epsilon();
            return std::abs(a.Left - b.Left) < epsilon && std::abs(a.Top - b.Top) < epsilon &&
                   std::abs(a.Width - b.Width) < epsilon && std::abs(a.Height - b.Height) < epsilon;
        }

        friend bool operator!=(const Rectangle& a, const Rectangle& b) { return !(a == b); }

        friend constexpr Rectangle operator*(const Rectangle& rect, double scale)
        {
            return Rectangle(rect.TopLeft() * scale, rect.Size() * scale);
        }

        friend constexpr Rectangle operator/(const Rectangle& rect, double scale)
        {
            return Rectangle(rect.TopLeft() / scale, rect.Size() / scale);
        }
    };
#pragma pack(pop)
} // namespace Voxium::Core
