#pragma once

#include <string>

#include "CoreMacros.h"

#include "Math/Gravity.h"

namespace Voxium::Core
{

    struct Vector;
    struct Size;

#pragma pack(push, 1)
    struct CORE_API Rectangle
    {
        double                 Left, Top, Width, Height;
        static const Rectangle Empty;

        constexpr Rectangle(double left, double top, double width, double height) :
            Left(left), Top(top), Width(width), Height(height)
        {}

        constexpr Rectangle() : Left(0), Top(0), Width(0), Height(0) {}
        Rectangle(const Vector& topleft, const Size& size);

        bool operator==(const Rectangle& other) const;
        bool operator!=(const Rectangle& other) const;

        constexpr bool     IsEmpty() const { return Width == 0 || Height == 0; }
        constexpr double   Bottom() const { return Top + Height; }
        constexpr void     SetBottom(double bottom) { Height = bottom - Top; }
        constexpr double   Right() const { return Left + Width; }
        constexpr void     SetRight(double right) { Width = right - Left; }
        constexpr double   CenterX() const { return (Left + Right()) / 2.0; }
        constexpr double   CenterY() const { return (Top + Bottom()) / 2.0; }
        bool               IsEqual(const Rectangle& other) const;
        Vector             Center() const;
        Vector             CenterSnapped() const;
        Rectangle          Clip(const Rectangle& outer) const;
        bool               Contains(const Vector& point) const;
        Vector             TopLeft() const;
        void               SetTopLeft(const Vector& v);
        Vector             TopRight() const;
        void               SetTopRight(const Vector& v);
        Vector             BottomLeft() const;
        Vector             BottomRight() const;
        void               SetBottomRight(const Vector& v);
        Voxium::Core::Size Size() const;
        void               SetSize(const Voxium::Core::Size& s);
        Rectangle          ApplyGravity(const Voxium::Core::Size& size, Gravity align) const;
        size_t             GetHashCode() const;
        std::string        ToString() const;

        constexpr Rectangle ApplyInsets(double leftInset, double topInset, double rightInset, double bottomInset) const
        {
            return Rectangle(
                Left + leftInset, Top + topInset, Width - leftInset - rightInset, Height - topInset - bottomInset);
        }

        constexpr Rectangle
        ApplyOutsets(double leftOutset, double topOutset, double rightOutset, double bottomOutset) const
        {
            return Rectangle(Left - leftOutset,
                             Top - topOutset,
                             Width + leftOutset + rightOutset,
                             Height + topOutset + bottomOutset);
        }

        constexpr Rectangle ApplyInsets(double h) const { return ApplyInsets(h, h, h, h); }
        constexpr Rectangle ApplyOutsets(double h) const { return ApplyOutsets(h, h, h, h); }
        constexpr Rectangle Inflate(double w, double h) const
        {
            return Rectangle(Left - w, Top - h, Width + 2 * w, Height + 2 * h);
        }
    };
#pragma pack(pop)

    CORE_API Rectangle operator*(const Rectangle& rect, double scale);
    CORE_API Rectangle  operator/(const Rectangle& rect, double scale);

} // namespace Voxium::Core
